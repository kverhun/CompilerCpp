#include "ParseTree.h"

#include "GrammarSymbol.h"
#include "Grammar.h"
#include "SymbolTable.h"

#include <vector>
#include <boost\optional.hpp>

#include <iostream>
#include <algorithm>

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

    //------------------------------------------------------------------------------
    // name to type map
    void _GetFunctionParametersIndexesList(std::map<size_t, size_t>& io_params, const ParseTree::_Node& i_node)
    {
        if (i_node.m_grammar_symbol.IsNonTerminal())
        {
            auto nonterm_info = i_node.m_grammar_symbol.GetNonterminalInfo();
            if (nonterm_info == NonTerminal("parameter-declaration"))
            {
                size_t type_idx = i_node.m_children[0].m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex();
                size_t name_idx = i_node.m_children[1].m_grammar_symbol.GetTerminalInfo().GetIndex();
                io_params[name_idx] = type_idx;
            }
            else
            {
                for (const auto& ch : i_node.m_children)
                    _GetFunctionParametersIndexesList(io_params, ch);
            }
        }
    }

    //------------------------------------------------------------------------------
    void _GetSymbolTables(std::vector<SymbolTable>& io_symbol_tables, size_t& io_current_table, const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, size_t i_parent_table = 0)
    {
        if (!i_node.m_grammar_symbol.IsNonTerminal())
            return;

        if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal("function-definition"))
        {
            auto func_decl = i_node.m_children[0];
            auto func_param_str = i_parsed_string[func_decl.m_children[0].m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;
            auto func_name = i_parsed_string[func_decl.m_children[1].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;

            auto func_params_node = func_decl.m_children[3];
            std::map<size_t, size_t> params_indexes;
            _GetFunctionParametersIndexesList(params_indexes, func_params_node);
            std::vector<std::pair<std::string, std::string>> params_list;
            for (auto p_idx : params_indexes)
                params_list.push_back(std::make_pair(i_parsed_string[p_idx.first].m_lexeme_value, i_parsed_string[p_idx.second].m_lexeme_value));

            if (!io_symbol_tables[io_current_table].AddFunction(func_name, func_param_str, params_list))
                throw std::logic_error("Symbol redefinition: " + func_name);

            auto func_body = i_node.m_children[1];
            auto body_statement = func_body.m_children[0];

            size_t cur_table_idx = io_symbol_tables.size();
            io_symbol_tables.push_back(SymbolTable(cur_table_idx, i_parent_table));

            for (auto p : params_list)
                if (!io_symbol_tables[cur_table_idx].AddVariable(p.first, p.second))
                    throw std::logic_error("Symbol redefinition: " + p.first);

            size_t current_table_cahced = io_current_table;
            io_current_table = cur_table_idx;
            for (auto child : body_statement.m_children)
                _GetSymbolTables(io_symbol_tables, io_current_table, child, i_parsed_string, cur_table_idx);
            io_current_table = current_table_cahced;
            return;

        }
        else if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal("compound-statement"))
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
                    throw std::logic_error("Symbol redefinition: " + param_name);
            }
        }

        for (auto child : i_node.m_children)
            _GetSymbolTables(io_symbol_tables, io_current_table, child, i_parsed_string, i_parent_table);

    }

    //------------------------------------------------------------------------------
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

namespace
{
    using TRpnIterator = std::list<std::string>::iterator;

    const std::string g_binary_expression_str = "expression";

    auto addtive_expression_to_rpn = [](const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator i_where)
    {

    };

    void _ExpandNodeBinaryExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_grammar_symbol.IsLambda())
            return;
        else if (i_node.m_children.size() == 1)
        {
            if (i_node.m_children[0].m_grammar_symbol.IsTerminal())
            {
                auto id_str = i_parsed_string[i_node.m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;
                i_where = i_rpn.insert(i_where, id_str);
                
            }
            else
                _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children[1].m_children[0].m_grammar_symbol.IsLambda())
        {
            _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else
        {
            auto operator_str = i_parsed_string[i_node.m_children[1].m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;
            i_where = i_rpn.insert(i_where, operator_str);
            _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
            _ExpandNodeBinaryExpression(i_node.m_children[1].m_children[1], i_parsed_string, i_rpn, i_where);
            _ExpandNodeBinaryExpression(i_node.m_children[1].m_children[2], i_parsed_string, i_rpn, i_where);
        }
    }

    void _ExpandNode(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_grammar_symbol.IsTerminal())
        {
//            i_rpn.insert(i_where, i_parsed_string[i_node.m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value);
        }

        else if (i_node.m_grammar_symbol.IsNonTerminal())
        {
            if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal(g_binary_expression_str))
                _ExpandNodeBinaryExpression(i_node, i_parsed_string, i_rpn, i_where);
            else
            {
                for (auto it = i_node.m_children.rbegin(); it != i_node.m_children.rend(); ++it)
                    _ExpandNode(*it, i_parsed_string, i_rpn, i_where);
            }
        }

    }
}

//------------------------------------------------------------------------------
TReversePolishNotation ParseTree::GetReversePolishNotation(const LexicalAnalysis::TParsedString& i_parsed_string) const
{
    TReversePolishNotation result;
    _ExpandNode(*m_root_node, i_parsed_string, result, result.begin());
    return result;
}
