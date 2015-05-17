#pragma once 

#include <vector>
#include <memory>
#include <ostream>

#include<Interfaces\LexicalAnalyzer\LexemeInfo.h>

namespace SyntaxAnalysis
{
    class Grammar;
    class SymbolTable;

    class ParseTree
    {
    public:
        ParseTree(const Grammar& i_grammar, const std::vector<size_t>& i_productions_used);

        std::vector<SymbolTable> GetSymbolTables(const LexicalAnalysis::TParsedString& i_parsed_string) const;

    public:
        struct _Node;

    private:
        std::shared_ptr<_Node> m_root_node;
    };
}
