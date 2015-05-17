#include "ParseTree.h"

#include "GrammarSymbol.h"
#include "Grammar.h"

#include <vector>
#include <boost\optional.hpp>

using namespace SyntaxAnalysis;


//------------------------------------------------------------------------------
struct ParseTree::_ParseTreeNode
{
    GrammarSymbol m_grammar_symbol;
    std::vector<ParseTree::_ParseTreeNode> m_children;
};

namespace
{
    boost::optional<ParseTree::_ParseTreeNode&> _GetLeftmostNonterminalNode(ParseTree::_ParseTreeNode& i_root_node)
    {
        if (i_root_node.m_grammar_symbol.IsNonTerminal() && i_root_node.m_children.empty())
            return i_root_node;
        
        for (auto& ch : i_root_node.m_children)
        {
            auto res = _GetLeftmostNonterminalNode(ch);
            if (res)
                return res;
        }

        return boost::none;
    }

    //------------------------------------------------------------------------------
    size_t _ExpandLeftmostNonterminal(ParseTree::_ParseTreeNode& i_root_node, const Grammar& i_grammar, size_t i_production_used)
    {
        auto leftmost_nonterminal = _GetLeftmostNonterminalNode(i_root_node);
        if (!leftmost_nonterminal)
            return false;

        auto production = i_grammar.GetProduction(leftmost_nonterminal->m_grammar_symbol)[i_production_used];

        leftmost_nonterminal->m_children.reserve(production.size());
        for (auto symbol : production)
            leftmost_nonterminal->m_children.push_back(ParseTree::_ParseTreeNode{ symbol, {} });

        return true;
    }

    //------------------------------------------------------------------------------
    void _GetIdentifierNamesFromIdentifierListNode(std::vector<std::string>& io_id_list, const ParseTree::_ParseTreeNode& i_node)
    {
        if (i_node.m_children.size() == 0)
        {
            if (i_node.m_grammar_symbol.IsTerminal() && i_node.m_grammar_symbol.GetTerminalInfo() == Terminal("ID"))
                io_id_list.push_back(i_node.m_grammar_symbol.GetTerminalInfo().GetValue());
        }
        else
        {
            for (const auto& child : i_node.m_children)
                _GetIdentifierNamesFromIdentifierListNode(io_id_list, child);
        }
    }

    //------------------------------------------------------------------------------
    void _AppendSymbolTableInfo(std::string& io_str, const ParseTree::_ParseTreeNode& i_node, size_t i_indend_level = 0)
    {
        if (!i_node.m_grammar_symbol.IsNonTerminal())
            return;

        if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal("compound-statement"))
        {
            io_str.append("\n");
            io_str.append("    ", i_indend_level);
            io_str.append("BLOCK BEGIN");
            for (auto child : i_node.m_children)
                _AppendSymbolTableInfo(io_str, child, i_indend_level + 1);
            io_str.append("\n");
            io_str.append("    ", i_indend_level);
            io_str.append("BLOCK END");
            return;
        }
        else if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal("variable-definition"))
        {
            std::string parameter_type = i_node.m_children[0].m_children[0].m_grammar_symbol.GetTerminalInfo().GetValue();

            auto& param_list_node = i_node.m_children[1];
            std::vector<std::string> param_names_list;
            _GetIdentifierNamesFromIdentifierListNode(param_names_list, param_list_node);

            for (const auto& param_name : param_names_list)
            {
                io_str.append("\n");
                io_str.append("    ", i_indend_level);
                io_str.append("VAR: ");
                io_str.append(param_name);
                io_str.append("; TYPE: ");
                io_str.append(parameter_type);
            }
        }
        
        for (auto gs : i_node.m_children)
            _AppendSymbolTableInfo(io_str, gs, i_indend_level);

    }
}

//------------------------------------------------------------------------------
ParseTree::ParseTree(const Grammar& i_grammar, const std::vector<size_t>& i_productions_used, const SyntaxAnalysis::TSyntaxAnalyzerInput& i_parsed_string)
: m_root_node(new _ParseTreeNode{ i_grammar.GetStartSymbol(), {} })
{
    for (auto i_idx : i_productions_used)
        _ExpandLeftmostNonterminal(*m_root_node, i_grammar, i_idx);
}

//------------------------------------------------------------------------------
std::string SyntaxAnalysis::ParseTree::GetSymbolTableString() const
{
    std::string result;

    result.append("GLOBAL BLOCK BEGIN\n");

    _AppendSymbolTableInfo(result, *m_root_node);

    result.append("GLOBAL BLOCK END\n");

    return result;
}
