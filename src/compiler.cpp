#include "compiler.h"
#include "exceptions.h"
#include "lexer.h"
#include "logger.h"
#include "parser.h"
#include "preprocessor.h"
#include "optimizer.h"
#include "analyzer.h"

#include <fstream>
#include <sstream>

namespace Lithium
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
        if (!source.is_open() || source.fail())
        {
            ThrowFatalException(
                    { "Failed to open source file", ErrorCode::InvalidSourceFile });
        }

        std::stringstream buffer;
        buffer << source.rdbuf();

        std::string file = buffer.str();

        PreProcessor preprocessor{ file };
        file = preprocessor.PreProcess();

        Lexer lexer{ file };
        std::vector<Token> tokens = lexer.Lex();

        std::cout << "\n";
        Logger::Info("Tokens\n");
        for (auto& e : tokens)
        {
            Logger::Info(Token::ToString(e));
        }

        Parser parser{ tokens };
        SyntaxTree ast = parser.Parse();

        std::cout << "\n";
        Logger::Info("AST\n");
        ast.Print();

        Analyzer analyzer{ ast };
        std::vector<Exception> exceptions = analyzer.Analyze();

        std::cout << "\n";

        for (auto& exception : exceptions)
            ThrowException(exception);

        std::cout << "\n";
        Logger::Info("Optimized\n");

        Optimizer optimizer{ ast };
        ast = optimizer.Optimize();

        ast.Print();

        // ByteCodeTranslator translator{ ast };
        // ByteCodeProgram* program = translator.Translate();
    }

}

