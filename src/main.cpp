#include "exceptions.h"
#include "logger.h"
#include "compiler.h"

#include <vector>

using namespace Pirut;

int main(int argc, char** argv)
{
    Logger::Init();
    Logger::Info("Logger setup complete");
    Logger::Info("Note: Compiler flags are currently ignored");

    if (argc == 1)
    {
        ThrowFatalException(
            Exception("No input files or arguments", ErrorCode::CmdLineInputException)
        );
    }

    std::vector<std::string> inputFiles{};
    std::vector<std::string> compilerFlags{};

    for (int i = 1; i < argc; i++)
    {
        if (*argv[i] == '-')
            compilerFlags.emplace_back(argv[i]);
        else
            inputFiles.emplace_back(argv[i]);
    }

    if (inputFiles.size() > 1)
    {
        ThrowException(
            Exception("Only one input file supported", ErrorCode::CmdLineInputException)
        );
    }
    else if (inputFiles.empty())
    {
        ThrowFatalException(
            Exception("No input files", ErrorCode::CmdLineInputException)
        );
    }

    Compiler compiler{ inputFiles, compilerFlags };
    compiler.CompileFiles();

}

