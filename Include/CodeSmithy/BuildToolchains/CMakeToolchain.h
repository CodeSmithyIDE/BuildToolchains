/*
    Copyright (c) 2020 Xavier Leclercq
    Released under the MIT License
    See https://github.com/CodeSmithyIDE/BuildToolchains/blob/master/LICENSE.txt
*/

#ifndef _CODESMITHYIDE_BUILDTOOLCHAINS_CMAKETOOLCHAIN_H_
#define _CODESMITHYIDE_BUILDTOOLCHAINS_CMAKETOOLCHAIN_H_

#include <Ishiko/Process.h>
#include <string>

namespace CodeSmithy
{

class CMakeToolchain
{
public:
    CMakeToolchain();

    void build(const std::string& makefilePath, const Ishiko::Process::Environment& environment) const;

private:
    std::string m_cmakePath;
};

}

#include "linkoptions.h"

#endif
