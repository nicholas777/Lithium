#include "exceptions.h"
#include "logger.h"

#include <spdlog/fmt/fmt.h>
#include <stdlib.h>

namespace Lithium
{

    static bool IsCodeException(ErrorCode code)
    {
        return code != ErrorCode::CmdLineInputException;
    }

    Exception::Exception(const char* msg, ErrorCode errorno,
            const std::string& file, uint32_t line)
        : Message(msg), Errno(errorno), 
        File(file), Line(line), Code(IsCodeException(errorno)) {}

    void ThrowFatalException(const Exception& exception)
    {
        if (exception.Code)
        {
            Logger::Fatal(fmt::format(
                    "Fatal exception in file {} at line {}: {}", 
                    exception.File, exception.Line, exception.Message));
        }
        else
        {
            Logger::Fatal(fmt::format(
                        "Fatal: {}", exception.Message));
        }
        exit(1);
    }

    void ThrowException(const Exception& exception)
    {
        if (exception.Code)
        {
            Logger::Error(fmt::format(
                    "Exception in file {} at line {}: {}", 
                    exception.File, exception.Line, exception.Message));
        }
        else
        {
            Logger::Error(fmt::format(
                        "Fatal: {}", exception.Message));
        }
        exit(1);
    }
}

