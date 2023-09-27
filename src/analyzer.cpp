#include "analyzer.h"
#include "exceptions.h"
#include "parser.h"
#include "lexer.h"

#include <algorithm>
#include <functional>
#include <spdlog/fmt/bundled/core.h>
#include <string_view>
#include <vector>

namespace Lithium
{

    struct Scope
    {
        std::vector<std::string> symbols;
        Scope* parent = nullptr;

        Scope() = default;
        Scope(Scope* p) : parent(p) {};
    };

    std::vector<Exception> Analyzer::Analyze()
    {
        std::vector<Exception> errors;
        Scope* currentScope = new Scope; // Global scope by default

        std::function<void(SyntaxTreeNode&)> analyzeNode = [&] (SyntaxTreeNode& child) {
            if (child.token.Type == TokenType::Symbol)
            {
                if (std::find(currentScope->symbols.begin(), currentScope->symbols.end(), child.token.Value) == currentScope->symbols.end())
                    errors.emplace_back(fmt::format("Symbol {} used before definition", child.token.Value).c_str(), ErrorCode::UseBeforeDeclaration);
            }
            else if (child.token.Type == TokenType::VarDecl)
            {
                if (std::find(currentScope->symbols.begin(), currentScope->symbols.end(), child.token.Value) != currentScope->symbols.end())
                    errors.emplace_back(fmt::format("Symbol {} has already been declared", child.token.Value).c_str(), ErrorCode::AlreadyDeclared);

                currentScope->symbols.push_back(child.token.Value);
            }

            std::for_each(child.children.begin(), child.children.end(), [&](SyntaxTreeNode& node) {
                analyzeNode(node);
            });
        };

        for (SyntaxTreeNode& node : m_Tree.Root.children)
        {
            analyzeNode(node);
        }

        delete currentScope;

        return errors;
    }

}
