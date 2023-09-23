#include "compiler.h"
#include "logger.h"
#include "preprocessor.h"

#include <fstream>
#include <sstream>

namespace Pirut
{
    
    Compiler::Compiler(const std::vector<std::string>& files,
            const std::vector<std::string>& flags)
    {
        m_Config.Files = std::move(files);
    }

    Compiler::~Compiler()
    {

    }

    void Compiler::CompileFiles()
    {
        std::ifstream source{ m_Config.Files[0] };
        std::stringstream buffer;
        buffer << source.rdbuf();

        std::string file = buffer.str();

        PreProcessor preprocessor{ file };
        file = preprocessor.PreProcess();

        Logger::Info(file);
    }

}
