#pragma once

#include "exceptions.h"
#include "parser.h"

#include <vector>

namespace Lithium
{

    class Analyzer
    {
    public:
        Analyzer(const SyntaxTree& tree)
            : m_Tree(tree) {};

        std::vector<Exception> Analyze();
        
    private:
        SyntaxTree m_Tree;
    };

}
