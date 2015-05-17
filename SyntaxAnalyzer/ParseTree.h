#pragma once 

#include <vector>
#include <memory>
#include <ostream>

#include <Interfaces\SyntaxAnalyzer\SyntaxAnalyzerInput.h>

namespace SyntaxAnalysis
{
    class Grammar;

    class ParseTree
    {
    public:
        ParseTree(const Grammar& i_grammar, const std::vector<size_t>& i_productions_used, const SyntaxAnalysis::TSyntaxAnalyzerInput& i_parsed_string);

        std::string GetSymbolTableString() const;
    public:
        struct _ParseTreeNode;

    private:
        std::shared_ptr<_ParseTreeNode> m_root_node;
    };
}
