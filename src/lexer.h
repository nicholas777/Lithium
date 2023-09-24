#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace Lithium
{

    enum class TokenType
    {
        None = 0,
        Integer, Float, String,
        OperatorPlus, OperatorMinus, OperatorMul, OperatorDiv,
        Symbol,
        Equals, Semicolon,
        LeftParen, RightParen
    };

    struct Token
    {
        TokenType Type = TokenType::None;
        std::string Value = "";

        Token() = default;

        Token(TokenType type)
            : Type(type) {};

        Token(TokenType type, std::string value)
            : Type(type), Value(value) {};

        static std::string ToString(const Token& token);

        bool IsNumeric();
        bool IsOperator();
        bool IsValue();

        bool IsHigherOtherOperator(const Token& other);
    };

    class Lexer
    {
    public:
        Lexer(const std::string& input)
            : m_Input(input) {};

        ~Lexer() = default;

        std::vector<Token> Lex();
    private:
        std::string m_Input;
        uint32_t m_Index = 0;

        Token ProcessNumber();
        Token ProcessString();
        Token ProcessSymbol();
    };

}
