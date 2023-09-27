#pragma once

#include "parser.h"

namespace Lithium
{

    class Optimizer
    {
    public:
        Optimizer(const SyntaxTree& tree)
            : m_Tree(tree) {};

        ~Optimizer() = default;

        SyntaxTree Optimize();
    private:
        SyntaxTree m_Tree;

        SyntaxTreeNode EvaluateTreeNode(SyntaxTreeNode& node);
    };

}
