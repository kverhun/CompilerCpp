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
    static size_t g_current_label_index = 1;

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
    g_current_label_index = 1;

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
    const std::string g_expression_str = "expression";
    const std::string g_assingnment_expression_str = "assingment-expression";
    const std::string g_primary_expression_str = "primary-expression";
    const std::string g_unary_expression_str = "unary-expression";
    const std::string g_compound_statement_str = "compound-statement";
    const std::string g_if_statement_str = "if-statement";
    const std::string g_while_statement_str = "while-statement";
    const std::string g_statement_sequence_str = "statement-sequence";
    const std::string g_statement_str = "statement";
    const std::string g_expression_statement = "expression-statement";
    const std::string g_function_defintion_str = "function-definition";

    const std::string g_goto_cmd = "GOTO";
    const std::string g_iffalse_cmd = "IFFALSE";
    const std::string g_func_decl_cmd = "FUNCDECL";
    const std::string g_func_call_cmd = "CALL";

    std::vector<GrammarSymbol> terminal_unary_ops = {
        Terminal("!"), Terminal("+"), Terminal("-"), Terminal("*"), Terminal("&"), Terminal("~")
    };

    std::map<std::string, std::string> g_unary_rpn_ops = { { "+", "UNPLUS" }, { "!", "NOT" }, { "-", "UNMINUS" }, { "*", "DEREF" }, { "&", "ADDR" }, { "~", "BITNOT" } };

    void _ExpandNodeBinaryExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where);
    void _ExpandPrimaryExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where);
    void _ExpandAssingmentExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where);
    void _ExpandNodeDirectly(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where);
    void _ExpandStatement(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where);
    void _ExpandStatementSequence(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where);


    //------------------------------------------------------------------------------
    std::string _GenerateLabel()
    {
        return "m" + std::to_string(g_current_label_index++);
    }

    //------------------------------------------------------------------------------
    void _ExpandNodeDirectly(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        auto id_str = i_parsed_string[i_node.m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;
        i_where = i_rpn.insert(i_where, id_str);
    }

    //------------------------------------------------------------------------------
    void _ExpandCompoundStatement(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children[1].m_children[0].m_grammar_symbol.IsLambda() == false)
        {
            i_where = i_rpn.insert(i_where, "end");
            _ExpandStatementSequence(i_node.m_children[1].m_children[0], i_parsed_string, i_rpn, i_where);
            i_where = i_rpn.insert(i_where, "begin");
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandWhileStatement(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        auto label1 = _GenerateLabel();
        auto label2 = _GenerateLabel();

        i_where = i_rpn.insert(i_where, label2 + ":");
        i_where = i_rpn.insert(i_where, g_goto_cmd);
        i_where = i_rpn.insert(i_where, label1);

        i_where = i_rpn.insert(i_where, g_iffalse_cmd);
        i_where = i_rpn.insert(i_where, label2);
        _ExpandAssingmentExpression(i_node.m_children[2].m_children[0], i_parsed_string, i_rpn, i_where);

        _ExpandStatement(i_node.m_children[4], i_parsed_string, i_rpn, i_where);
        
        i_where = i_rpn.insert(i_where, label1 + ":");
    }

    //------------------------------------------------------------------------------
    void _ExpandIfStatement(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children.size() == 7)
        {
            auto label1 = _GenerateLabel();
            auto label2 = _GenerateLabel();
            
            i_where = i_rpn.insert(i_where, label2 + ":");
            _ExpandStatement(i_node.m_children[6], i_parsed_string, i_rpn, i_where);
            i_where = i_rpn.insert(i_where, label1 + ":");
            i_where = i_rpn.insert(i_where, g_goto_cmd);
            i_where = i_rpn.insert(i_where, label2);
            _ExpandStatement(i_node.m_children[4], i_parsed_string, i_rpn, i_where);
            i_where = i_rpn.insert(i_where, g_iffalse_cmd);
            i_where = i_rpn.insert(i_where, label1);
            _ExpandAssingmentExpression(i_node.m_children[2].m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children.size() == 5)
        {
            auto label1 = _GenerateLabel();

            i_where = i_rpn.insert(i_where, label1 + ":");
            _ExpandStatement(i_node.m_children[4], i_parsed_string, i_rpn, i_where);
            i_where = i_rpn.insert(i_where, g_iffalse_cmd);
            i_where = i_rpn.insert(i_where, label1);
            _ExpandAssingmentExpression(i_node.m_children[2].m_children[0], i_parsed_string, i_rpn, i_where);
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandStatementSequence(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children.size() == 2)
            _ExpandStatementSequence(i_node.m_children[1], i_parsed_string, i_rpn, i_where);
        _ExpandStatement(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
    }

    //------------------------------------------------------------------------------
    void _ExpandStatement(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children[0].m_grammar_symbol.GetNonterminalInfo() == NonTerminal(g_expression_statement))
        {
            if (i_node.m_children[0].m_children[0].m_grammar_symbol.IsLambda() == false)
                _ExpandAssingmentExpression(i_node.m_children[0].m_children[0].m_children[0].m_children[0], i_parsed_string, i_rpn, i_where);
            else
                return;
        }
        else if (i_node.m_children[0].m_grammar_symbol.GetNonterminalInfo() == NonTerminal(g_compound_statement_str))
        {
            _ExpandCompoundStatement(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children[0].m_grammar_symbol.GetNonterminalInfo() == NonTerminal(g_if_statement_str))
        {
            _ExpandIfStatement(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children[0].m_grammar_symbol.GetNonterminalInfo() == NonTerminal(g_while_statement_str))
        {
            _ExpandWhileStatement(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else
        {
            // other statements
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandAssingmentExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children.size() == 1)
            _ExpandNodeBinaryExpression(i_node, i_parsed_string, i_rpn, i_where);
        else if (i_node.m_children.size() == 3)
        {
            _ExpandNodeDirectly(i_node.m_children[1].m_children[0], i_parsed_string, i_rpn, i_where);
            _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
            _ExpandAssingmentExpression(i_node.m_children[2], i_parsed_string, i_rpn, i_where);
        }
    }

    size_t _CaclulateExpressionNumber(const ParseTree::_Node& i_node)
    {
        if (i_node.m_children.size() == 1)
            return 1;
        else
            return 1 + _CaclulateExpressionNumber(i_node.m_children[2]);
    }

    //------------------------------------------------------------------------------
    void _ExpandExpressionList(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children.size() == 1)
            _ExpandAssingmentExpression(i_node.m_children[0].m_children[0], i_parsed_string, i_rpn, i_where);
        else if (i_node.m_children.size() == 3)
        {
            _ExpandExpressionList(i_node.m_children[2], i_parsed_string, i_rpn, i_where);
            _ExpandAssingmentExpression(i_node.m_children[0].m_children[0], i_parsed_string, i_rpn, i_where);
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandPrimaryExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_children.size() == 1)
        {
            // literal or identifier
            _ExpandNodeDirectly(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children.size() == 3)
        {
            // ( expr )
            _ExpandNodeBinaryExpression(i_node.m_children[1], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children.size() == 4)
        {
            // function call

            i_where = i_rpn.insert(i_where, g_func_call_cmd);

            size_t arg_num;
            if (i_node.m_children[2].m_children[0].m_grammar_symbol.IsLambda() == false)
                arg_num = _CaclulateExpressionNumber(i_node.m_children[2].m_children[0]);
            else
                arg_num = 0;
            
            i_where = i_rpn.insert(i_where, std::to_string(arg_num));

            if (arg_num)
                _ExpandExpressionList(i_node.m_children[2].m_children[0], i_parsed_string, i_rpn, i_where);

            // func name
            i_where = i_rpn.insert(i_where, i_parsed_string[i_node.m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value);
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandNodeUnaryExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    { 
        if (i_node.m_children.size() == 1)
            _ExpandPrimaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        else if (i_node.m_children.size() == 2)
        {
            auto op_str = i_parsed_string[i_node.m_children[0].m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;
            i_where = i_rpn.insert(i_where, g_unary_rpn_ops[op_str]);

            _ExpandPrimaryExpression(i_node.m_children[1], i_parsed_string, i_rpn, i_where);
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandNodeBinaryExpression(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_grammar_symbol.IsLambda())
            return;
        else if (i_node.m_grammar_symbol.GetNonterminalInfo() == g_primary_expression_str)
            _ExpandPrimaryExpression(i_node, i_parsed_string, i_rpn, i_where);
        else if (i_node.m_grammar_symbol.GetNonterminalInfo() == g_unary_expression_str)
            _ExpandNodeUnaryExpression(i_node, i_parsed_string, i_rpn, i_where);
        else if (i_node.m_children.size() == 1)
        {
            if (i_node.m_children[0].m_grammar_symbol.IsTerminal())
                _ExpandNodeDirectly(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
            else
                _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        }
        else if (i_node.m_children[1].m_children[0].m_grammar_symbol.IsLambda())
            _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
        
        else
        {
            auto operator_str = i_parsed_string[i_node.m_children[1].m_children[0].m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value;
            i_where = i_rpn.insert(i_where, operator_str);
            _ExpandNodeBinaryExpression(i_node.m_children[0], i_parsed_string, i_rpn, i_where);
            _ExpandNodeBinaryExpression(i_node.m_children[1].m_children[1], i_parsed_string, i_rpn, i_where);
            _ExpandNodeBinaryExpression(i_node.m_children[1].m_children[2], i_parsed_string, i_rpn, i_where);
        }
    }

    //------------------------------------------------------------------------------
    void _ExpandNode(const ParseTree::_Node& i_node, const LexicalAnalysis::TParsedString& i_parsed_string, TReversePolishNotation& i_rpn, TRpnIterator& i_where)
    {
        if (i_node.m_grammar_symbol.IsTerminal())
        {
            // not to happen
            // i_rpn.insert(i_where, i_parsed_string[i_node.m_grammar_symbol.GetTerminalInfo().GetIndex()].m_lexeme_value);
            //throw(std::logic_error("terminal reached"));
        }

        else if (i_node.m_grammar_symbol.IsNonTerminal())
        {
            if (i_node.m_grammar_symbol.GetNonterminalInfo() == NonTerminal(g_compound_statement_str))
                _ExpandCompoundStatement(i_node, i_parsed_string, i_rpn, i_where);
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
