#include "parser.h"
#include "exceptions.h"
#include "lexer.h"

namespace Lithium 
{

    SyntaxTree Parser::Parse()
    {
        m_Result.Root.token = Token(TokenType::Root);
        SyntaxTreeNode& currentToken = m_Result.Root;
        
        while (m_Index < m_Tokens.size())
        {
            currentToken.children.push_back(VisitToken());
            if (currentToken.children.back().token.Type == TokenType::Semicolon ||
                    currentToken.children.back().token.Type == TokenType::RightParen)
                currentToken.children.pop_back();

            m_Index++;
        }

        return m_Result;
    }

    SyntaxTreeNode Parser::VisitToken()
    {
        Token& token = m_Tokens[m_Index];

        if (token.IsNumeric() || token.Type == TokenType::OperatorMinus)
            return VisitNumberToken();
        else if (token.Type == TokenType::LeftParen)
            return VisitParenToken();
        else if (token.Type == TokenType::RightParen)
            return VisitParenToken();
        else if (token.Type == TokenType::Semicolon)
            return { token };
        else if (token.Type == TokenType::VarDecl)
            return VisitVariableToken();
        else if (token.IsLiteral())
            return { token };
        else if (token.Type == TokenType::Symbol)
            return { token };

        return {};
    }

    SyntaxTreeNode Parser::VisitVariableToken()
    {
        if (m_Index >= m_Tokens.size() - 2)
        {
            ThrowException(
                Exception("Invalid variable declaration", ErrorCode::InvalidVariableDecl));

            return {};
        }
        else if (m_Tokens[m_Index+1].Type != TokenType::Symbol)
        {
            ThrowException(
                Exception("Invalid variable declaration", ErrorCode::InvalidVariableDecl));

            return {};
        }

        uint32_t nameIndex = m_Index + 1;
        m_Index += 2;
        if (m_Tokens[m_Index].Type == TokenType::Equals)
        {
            m_Index++;
            SyntaxTreeNode value = VisitToken();

            if (!value.token.IsValue())
            {
                ThrowException(
                    Exception("Invalid variable declaration", ErrorCode::InvalidVariableDecl));
            }

            SyntaxTreeNode node;
            node.token = { TokenType::VarDecl, m_Tokens[nameIndex].Value };
            node.children.push_back(value);
            return node;
        }
        else if (m_Tokens[m_Index].Type == TokenType::Semicolon)
        {
            SyntaxTreeNode node;
            node.token = { TokenType::VarDecl, m_Tokens[m_Index-1].Value };
            return node;
        }
        else
        {
            ThrowException(
                Exception("Invalid variable declaration", ErrorCode::InvalidVariableDecl));

            return {};
        }

        return {};
    }

    SyntaxTreeNode Parser::VisitParenToken()
    {
        Token& token = m_Tokens[m_Index];

        if (token.Type == TokenType::LeftParen)
        {
            m_Parenthesis++;
            m_Index++;

            SyntaxTreeNode result{ token };
            result.children.push_back(VisitToken());
            
            return result;
        }
        else if (token.Type == TokenType::RightParen)
        {
            m_Parenthesis--;
            return { token };
        }

        return {};
    }

    SyntaxTreeNode Parser::VisitOperatorToken()
    {
        Token& operand1 = m_Tokens[m_Index-1];
        Token& op = m_Tokens[m_Index];
        
        if (m_Index == m_Tokens.size() - 1)
        {
            ThrowException(
                Exception("Invalid Math expression", ErrorCode::InvalidMath));

            return {};
        }
        
        if (!m_Tokens[m_Index+1].IsNumeric() && m_Tokens[m_Index+1].Type != TokenType::LeftParen)
        {
            ThrowException(
                Exception("Invalid Math expression", ErrorCode::InvalidMath));

            return {};
        }

        SyntaxTreeNode result{ op };

        m_Index++;
        SyntaxTreeNode other = VisitToken();
        
        if (other.token.IsNumeric())
        {
            result.children.push_back({ operand1 });
            result.children.push_back(std::move(other));

            return result;
        }
        else if (other.token.IsOperator())
        {
            if (other.token.IsHigherOtherOperator(op))
            {
                result.children.push_back({ operand1 });
                result.children.push_back(std::move(other));
                return result;
            }

            SyntaxTreeNode& operand2 = other.children[0];
            result.children.push_back({ operand1 });
            result.children.push_back(operand2);

            SyntaxTreeNode operand3 = other.children[1];
            other.children.clear();
            other.children.push_back(result);
            other.children.push_back(operand3);

            return other;
        }
        else if (other.token.Type == TokenType::LeftParen)
        {
            result.children.push_back({ operand1 });
            result.children.push_back(other.children[0]);

            return result;
        }

        return {};
    }

    SyntaxTreeNode Parser::VisitNumberToken()
    {
        Token& token = m_Tokens[m_Index];

        if (token.IsNumeric())
        {
            if (m_Index == m_Tokens.size() - 1)
                return { token };

            else if (!m_Tokens[m_Index + 1].IsOperator())
                return { token };

            // m_Index + 1 is an operator
            m_Index++;
            return VisitOperatorToken();
        }
        else if (token.Type == TokenType::OperatorMinus)
        {
            if (m_Index == m_Tokens.size() - 1)
            {
                ThrowException(
                    Exception("Invalid Math expression", ErrorCode::InvalidMath));

                return {};
            }

            else if (!m_Tokens[m_Index+1].IsNumeric())
            {
                // if (m_Tokens[m_Index+1].type == TokenType::LeftParen)

                ThrowException(
                    Exception("Invalid Math expression", ErrorCode::InvalidMath));

                return {};
            }

            SyntaxTreeNode result{ m_Tokens[m_Index] };
            result.children.push_back(m_Tokens[m_Index+1]);
            return result;
        }
        else 
        {
            ThrowException(
                Exception("Invalid Math expression", ErrorCode::InvalidMath));

            return {};
        }
    }
}
