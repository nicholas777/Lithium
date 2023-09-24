#pragma once

#include <vector>
#include <string>

namespace Lithium
{

    struct CompilerConfig
    {
        std::vector<std::string> Files;
    };

    class Compiler
    {
    public:
        Compiler(const std::vector<std::string>& files,
                const std::vector<std::string>& flags);

        ~Compiler();

        void CompileFiles();
    private:
        CompilerConfig m_Config;
    };

}
