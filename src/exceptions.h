#pragma once

#include <string>

namespace Lithium 
{

    enum class ErrorCode
    {
        None = 0,
        CmdLineInputException, InvalidSourceFile,
        InvalidString,
        InvalidMath,
        InvalidVariableDecl, UseBeforeDeclaration, AlreadyDeclared
    };

    struct Exception
    {
        std::string Message;
        ErrorCode Errno;
        
        std::string File = "";
        uint32_t Line = 0;

        bool Code;

        Exception(const char* msg, ErrorCode errorno, 
                const std::string& file = "", uint32_t line = 0);
    };

    [[ noreturn ]] void ThrowFatalException(const Exception& exception);
    void ThrowException(const Exception& exception);
}

