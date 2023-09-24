#include "lexer.h"
#include "exceptions.h"
#include "logger.h"

#include <cctype>
#include <spdlog/fmt/bundled/core.h>
#include <unordered_map>
#include <vector>

namespace Lithium
{

    bool Token::IsHigherOtherOperator(const Token& other)
    {
        return (Type == TokenType::OperatorMul && other.Type == TokenType::OperatorPlus) ||
            (Type == TokenType::OperatorMul && other.Type == TokenType::OperatorMinus) ||
            (Type == TokenType::OperatorDiv && other.Type == TokenType::OperatorPlus) ||
            (Type == TokenType::OperatorDiv && other.Type == TokenType::OperatorMinus);
    }

    bool Token::IsNumeric()
    {
        return Type == TokenType::Integer || Type == TokenType::Float;
    }

    bool Token::IsOperator()
    {
        return Type == TokenType::OperatorPlus
            || Type == TokenType::OperatorMinus
            || Type == TokenType::OperatorDiv
            || Type == TokenType::OperatorMul;
    }

    bool Token::IsValue()
    {
        return IsNumeric() || Type == TokenType::String;
    }

    static std::unordered_map<TokenType, const char*> TokenTypeToString = {
        { TokenType::None, "None" },
        { TokenType::String, "String" },
        { TokenType::Symbol, "Symbol" },
        { TokenType::Semicolon, "Semicolon" },
        { TokenType::Equals, "Equals" },
        { TokenType::OperatorPlus, "OperatorPlus" },
        { TokenType::OperatorMinus, "OperatorMinus" },
        { TokenType::OperatorMul, "OperatorMul" },
        { TokenType::OperatorDiv, "OperatorDiv" },
        { TokenType::Integer, "Integer" },
        { TokenType::Float, "Float" },
        { TokenType::LeftParen, "LeftParen" },
        { TokenType::RightParen, "RightParen" },
    };

    std::string Token::ToString(const Token& token)
    {
        return fmt::format("Type: {}, Value: {}", 
                TokenTypeToString[token.Type],
                token.Value);
    }

    std::vector<Token> Lexer::Lex()
    {
        std::vector<Token> tokens{};

        while (m_Index < m_Input.size())
        {
            char c = m_Input[m_Index];
            if (isdigit(c))
            {
                tokens.push_back(ProcessNumber());
                continue;
            }
            else if (c == '"')
            {
                tokens.push_back(ProcessString());
                continue;
            }
            else if (isalpha(c) || c == '_')
            {
                tokens.push_back(ProcessSymbol());
                continue;
            }

            if (c == ';')
                tokens.push_back(Token(TokenType::Semicolon));
            else if (c == '=')
                tokens.push_back(Token(TokenType::Equals));
            else if (c == '+')
                tokens.push_back(Token(TokenType::OperatorPlus));
            else if (c == '-')
                tokens.push_back(Token(TokenType::OperatorMinus));
            else if (c == '*')
                tokens.push_back(Token(TokenType::OperatorMul));
            else if (c == '/')
                tokens.push_back(Token(TokenType::OperatorDiv));
            else if (c == '(')
                tokens.push_back(Token(TokenType::LeftParen));
            else if (c == ')')
                tokens.push_back(Token(TokenType::RightParen));

            
            m_Index++;
        }

        return tokens;
    }

    Token Lexer::ProcessString()
    {
        size_t start = m_Index;
        size_t end = m_Input.find('"', start + 1);

        if (end == std::string::npos)
        {
            ThrowException(
                    Exception("Unterminated string", ErrorCode::InvalidString)
            );

            m_Index++;
            return Token();
        }

        m_Index = end + 1;
        return Token(TokenType::String, m_Input.substr(start, end - start + 1));
    }

    Token Lexer::ProcessSymbol()
    {
        std::string result = "";
        while (isalpha(m_Input[m_Index]) || isdigit(m_Input[m_Index]) || m_Input[m_Index] == '_')
        {
            result += m_Input[m_Index];
            m_Index++;
        }

        return Token(TokenType::Symbol, result);
    }

    Token Lexer::ProcessNumber()
    {
        std::string result = "";
        bool isFloat = false;

        while (isdigit(m_Input[m_Index]) || (m_Input[m_Index] == '.' && !isFloat))
        {
            if (m_Input[m_Index] == '.')
            {
                isFloat = true;
                result += '.';
            }
            else
            {
                result += m_Input[m_Index];
            }

            m_Index++;
        }

        return isFloat 
            ? Token(TokenType::Float, result) 
            : Token(TokenType::Integer, result);
    }

}
