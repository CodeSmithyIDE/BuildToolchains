/*
    Copyright (c) 2020 Xavier Leclercq
    Released under the MIT License
    See https://github.com/CodeSmithyIDE/BuildToolchains/blob/master/LICENSE.txt
*/

#include "CMakeToolchain.h"
#include "BuildToolchainErrorCategory.h"

using namespace Ishiko::Process;

namespace CodeSmithy
{

namespace
{

std::string CreateGenerationCommandLine(const std::string& cmakePath, const std::string& makefilePath,
    const CMakeGenerationOptions& options)
{
    std::string commandLine = cmakePath;
    boost::optional<std::string> generatorName = options.generatorName();
    if (generatorName)
    {
        commandLine.append(" -G \"");
        commandLine.append(*generatorName);
        commandLine.append("\"");
    }
    commandLine.append(" -S ");
    commandLine.append(boost::filesystem::path(makefilePath).parent_path().string());
    commandLine.append(" -B ");
    commandLine.append(boost::filesystem::path(makefilePath).parent_path().string());
    for (const std::pair<std::string, std::string>& cacheEntry : options.cacheEntries())
    {
        commandLine.append(" -D");
        commandLine.append(cacheEntry.first);
        commandLine.append("=");
        commandLine.append(cacheEntry.second);
    }
    return commandLine;
}

std::string CreateBuildCommandLine(const std::string& cmakePath, const std::string& makefilePath)
{
    std::string commandLine = cmakePath;
    commandLine.append(" --build ");
    commandLine.append(boost::filesystem::path(makefilePath).parent_path().string());
    return commandLine;
}

}

CMakeToolchain::CMakeToolchain()
    : m_cmakePath("cmake")
{
}

void CMakeToolchain::generate(const std::string& makefilePath, const Ishiko::Process::Environment& environment) const
{
    CMakeGenerationOptions options;
    generate(makefilePath, options, environment);
}

void CMakeToolchain::generate(const std::string& makefilePath, const CMakeGenerationOptions& options,
    const Ishiko::Process::Environment& environment) const
{
    std::string  commandLine = CreateGenerationCommandLine(m_cmakePath, makefilePath, options);
    ChildProcess process = ChildProcess::Spawn(commandLine, environment);
    process.waitForExit();
    int exitCode = process.exitCode();
    if (exitCode != 0)
    {
        Throw(BuildToolchainErrorCategory::eBuildError, "Process launched by " + commandLine + " exited with code "
            + std::to_string(exitCode), __FILE__, __LINE__);
    }
}

void CMakeToolchain::build(const std::string& makefilePath, const Environment& environment) const
{
    std::string commandLine = CreateBuildCommandLine(m_cmakePath, makefilePath);
    ChildProcess process = ChildProcess::Spawn(commandLine, environment);
    process.waitForExit();
    int exitCode = process.exitCode();
    if (exitCode != 0)
    {
        Throw(BuildToolchainErrorCategory::eBuildError, "Process launched by " + commandLine + " exited with code "
            + std::to_string(exitCode), __FILE__, __LINE__);
    }
}

}
