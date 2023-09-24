#pragma once

#include "lexer.h"
#include "logger.h"

#include <cstdio>
#include <iostream>
#include <spdlog/fmt/bundled/core.h>

namespace Lithium 
{

    struct SyntaxTreeNode
    {
        Token token;
        std::vector<SyntaxTreeNode> children;

        SyntaxTreeNode(const Token& tk)
            : token(tk) {};

        SyntaxTreeNode() = default;

        void Print(int recurion = 0)
        {
            for (int i = 0; i < recurion; i++)
                putchar('\t');
            Logger::Info(fmt::format("Token: {}:", Token::ToString(token)));

            for (SyntaxTreeNode& node : children)
                node.Print(recurion + 1);
        }
    };

    struct SyntaxTree
    {
        SyntaxTreeNode Root;

        void Print()
        {
            Root.Print();
        }
    };

    class Parser
    {
    public:
        Parser(const std::vector<Token>& tokens)
            : m_Tokens(tokens) {};

        ~Parser() = default;

        SyntaxTree Parse();
    private:
        std::vector<Token> m_Tokens;
        SyntaxTree m_Result;
        SyntaxTreeNode& m_CurrentToken = m_Result.Root;

        uint32_t m_Index = 0;
        uint32_t m_Parenthesis = 0;

        SyntaxTreeNode VisitToken();
        SyntaxTreeNode VisitParenToken();
        SyntaxTreeNode VisitNumberToken();
        SyntaxTreeNode VisitOperatorToken();
    };

}
