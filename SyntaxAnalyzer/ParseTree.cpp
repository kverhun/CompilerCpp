#include "ParseTree.h"

#include "GrammarSymbol.h"
#include "Grammar.h"
#include "SymbolTable.h"

#include <vector>
#include <boost\optional.hpp>

#include <iostream>

using namespace SyntaxAnalysis;


//------------------------------------------------------------------------------
struct ParseTree::_Node
{
    GrammarSymbol m_grammar_symbol;
    std::vector<ParseTree::_Node> m_children;
};

namespace
{
    //------------------------------------------------------------------------------
    boost::optional<ParseTree::_Node&> _GetLeftmostNonterminalNode(ParseTree::_Node& i_root_node)
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
    size_t _ExpandLeftmostNonterminal(ParseTree::_Node& i_root_node, const Grammar& i_grammar, size_t i_production_used)
    {
        auto leftmost_nonterminal = _GetLeftmostNonterminalNode(i_root_node);
        if (!leftmost_nonterminal)
            return false;

        auto production = i_grammar.GetProduction(leftmost_nonterminal->m_grammar_symbol)[i_production_used];

        leftmost_nonterminal->m_children.reserve(production.size());
        for (auto symbol : production)
            leftmost_nonterminal->m_children.push_back(ParseTree::_Node{ symbol, {} });
        
        return true;
    }

    //------------------------------------------------------------------------------
    void _GetIdentifierNamesFromIdentifierListNode(std::vector<std::string>& io_id_list, const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string)
    {
        if (i_node.m_children.size() == 0)
        {
            if (i_node.m_grammar_symbol.IsTerminal() && i_node.m_grammar_symbol.GetTerminalInfo() == Terminal("ID"))
                io_id_list.push_back(i_parsed_string[i_node.m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value);
        }
        else
        {
            for (const auto& child : i_node.m_children)
                _GetIdentifierNamesFromIdentifierListNode(io_id_list, child, i_parsed_string);
        }
    }

    void _GetSymbolTables(std::vector<SymbolTable>& io_symbol_tables, size_t& io_current_table, const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, size_t i_parent_table = 0)
    {
        if (!i_node.m_grammar_symbol.IsNonTerminal())
            return;

        if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal("compound-statement"))
        {
            size_t cur_table_idx = io_symbol_tables.size();
            io_symbol_tables.push_back(SymbolTable(cur_table_idx, i_parent_table));
            size_t current_table_cahced = io_current_table;
            io_current_table = cur_table_idx;
            for (auto child : i_node.m_children)
                _GetSymbolTables(io_symbol_tables, io_current_table, child, i_parsed_string, cur_table_idx);
            io_current_table = current_table_cahced;
            return;
        }
        else if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal("variable-definition"))
        {
            size_t parameter_idx = i_node.m_children[0].m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex();
            std::string parameter_type = i_parsed_string[parameter_idx].m_lexeme_value;

            auto& param_list_node = i_node.m_children[1];
            std::vector<std::string> param_names_list;
            _GetIdentifierNamesFromIdentifierListNode(param_names_list, param_list_node, i_parsed_string);

            for (const auto& param_name : param_names_list)
            {
                if (!io_symbol_tables[io_current_table].AddVariable(param_name, parameter_type))
                    throw std::logic_error("parameter redefinition: " + param_name + " of type " + parameter_type);
            }
        }

        for (auto child : i_node.m_children)
            _GetSymbolTables(io_symbol_tables, io_current_table, child, i_parsed_string, i_parent_table);

    }

    void _FixupTerminalIndexes(size_t& io_next_idx, ParseTree::_Node& i_node)
    {
        if (i_node.m_grammar_symbol.IsTerminal())
        {
            i_node.m_grammar_symbol.GetTerminalInfo().SetIndex(io_next_idx++);
            return;
        }
        
        for (auto& ch : i_node.m_children)
            _FixupTerminalIndexes(io_next_idx, ch);
    }
    

}

//------------------------------------------------------------------------------
ParseTree::ParseTree(const Grammar& i_grammar, const std::vector<size_t>& i_productions_used)
: m_root_node(new _Node{ i_grammar.GetStartSymbol(), {} })
{
    for (auto i_idx : i_productions_used)
        _ExpandLeftmostNonterminal(*m_root_node, i_grammar, i_idx);

    size_t next_term_idx = 0;
    _FixupTerminalIndexes(next_term_idx, *m_root_node);
}

//------------------------------------------------------------------------------
std::vector<SymbolTable> ParseTree::GetSymbolTables(const LexicalAnalysis::TParsedString& i_parsed_string) const
{
    std::vector<SymbolTable> res;
    res.push_back(SymbolTable(0, 0));
    size_t current_table = 0;

    _GetSymbolTables(res, current_table, *m_root_node, i_parsed_string);

    return res;
}
