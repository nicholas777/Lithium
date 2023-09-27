#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace Lithium
{

    enum class TokenType
    {
        None = 0, Root,
        Integer, Float, String,
        OperatorPlus, OperatorMinus, OperatorMul, OperatorDiv,
        Symbol,
        Equals, Semicolon,
        LeftParen, RightParen,
        VarDecl, FnDecl
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

        bool IsNumeric() const;
        bool IsOperator() const;
        bool IsValue() const;
        bool IsLiteral() const;

        bool IsHigherOtherOperator(const Token& other) const;
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
