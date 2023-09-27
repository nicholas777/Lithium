#include "optimizer.h"
#include "logger.h"
#include "parser.h"
#include <string>

namespace Lithium
{

    SyntaxTree Optimizer::Optimize()
    {
        for (auto& node : m_Tree.Root.children)
            node = EvaluateTreeNode(node);

        return m_Tree;
    }

    SyntaxTreeNode EvaluateConstMathExpression(const SyntaxTreeNode& node)
    {

        if (node.children.size() == 1 && node.token.Type == TokenType::OperatorMinus)
        {
            SyntaxTreeNode result;

            result.token = node.children[0].token.Type == TokenType::Integer
                ? Token(TokenType::Integer, std::to_string(-std::stoi(node.children[0].token.Value)))
                : Token(TokenType::Float, std::to_string(-std::stof(node.children[0].token.Value)));

            return result;
        }
        else if (node.token.IsNumeric())
        {
            return node;
        }
        else if (node.token.IsOperator())
        {
            SyntaxTreeNode operand1 = EvaluateConstMathExpression(node.children[0]);
            SyntaxTreeNode operand2 = EvaluateConstMathExpression(node.children[1]);

            SyntaxTreeNode result;
            result.token.Type = operand1.token.Type == TokenType::Float && operand2.token.Type == TokenType::Float
                ? TokenType::Float : TokenType::Integer;

            if (result.token.Type == TokenType::Float)
            {
                switch (node.token.Type)
                {
                    case TokenType::OperatorPlus:
                        result.token.Value = std::to_string(
                                std::stof(operand1.token.Value) +
                                std::stof(operand2.token.Value));
                    case TokenType::OperatorMinus:
                        result.token.Value = std::to_string(
                                std::stof(operand1.token.Value) -
                                std::stof(operand2.token.Value));
                    case TokenType::OperatorMul:
                        result.token.Value = std::to_string(
                                std::stof(operand1.token.Value) *
                                std::stof(operand2.token.Value));
                    case TokenType::OperatorDiv:
                        result.token.Value = std::to_string(
                                std::stof(operand1.token.Value) /
                                std::stof(operand2.token.Value));
                }
            }
            else
            {
                switch (node.token.Type)
                {
                    case TokenType::OperatorPlus:
                        result.token.Value = std::to_string(
                                std::stoi(operand1.token.Value) +
                                std::stoi(operand2.token.Value));
                        break;
                    case TokenType::OperatorMinus:
                        result.token.Value = std::to_string(
                                std::stoi(operand1.token.Value) -
                                std::stoi(operand2.token.Value));
                        break;
                    case TokenType::OperatorMul:
                        result.token.Value = std::to_string(
                                std::stoi(operand1.token.Value) *
                                std::stoi(operand2.token.Value));
                        break;
                    case TokenType::OperatorDiv:
                        result.token.Value = std::to_string(
                                std::stoi(operand1.token.Value) /
                                std::stoi(operand2.token.Value));
                        break;
                }
            }

            return result;
        }

        return {};
    }

    SyntaxTreeNode Optimizer::EvaluateTreeNode(SyntaxTreeNode& node)
    {
        if (node.token.IsOperator())
        {
            return EvaluateConstMathExpression(node);
        }
        else 
        {
            for (SyntaxTreeNode& child : node.children)
                child = EvaluateTreeNode(child);
        }
        
        return node;
    }

}
