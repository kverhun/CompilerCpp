#include "SyntaxAnalyzer.h"

#include <functional>

using namespace SyntaxAnalysis;

namespace
{
    typedef std::vector<GrammarSymbol> TParserState;

    bool _Terminal(size_t& io_next_index, const TSyntaxAnalyzerInput& i_parsed_string, const GrammarSymbol& i_grammar_symbol)
    {
        if (io_next_index >= i_parsed_string.size())
            return false;

        bool res = i_parsed_string[io_next_index] == i_grammar_symbol.GetTerminalInfo();
        ++io_next_index;
        return res;
    }

}

//------------------------------------------------------------------------------
SyntaxAnalyzer::SyntaxAnalyzer(const Grammar& i_grammar)
: m_grammar(i_grammar)
{

}

//------------------------------------------------------------------------------
bool SyntaxAnalyzer::Analyze(const TSyntaxAnalyzerInput& i_input)
{
    std::vector<size_t> v;
    return Analyze(v, i_input);
}

//------------------------------------------------------------------------------
bool SyntaxAnalyzer::Analyze(std::vector<size_t>& o_productions_used, const TSyntaxAnalyzerInput& i_input)
{
    size_t current_index = 0;

    auto start_symbol = m_grammar.GetStartSymbol();
    auto start_symbol_productions = m_grammar.GetProduction(start_symbol);

    bool res = _TryAllProductions(current_index, o_productions_used, i_input, start_symbol_productions);
    if (current_index != i_input.size())
        return false;

    return res;
}

//------------------------------------------------------------------------------
bool SyntaxAnalyzer::_TryProduction(size_t& io_next_index, std::vector<size_t>& o_productions_used, const TSyntaxAnalyzerInput& i_parsed_string, const TGrammarSymbolSequence& i_symbol_seq)
{
    bool res = true;
    for (auto gs : i_symbol_seq)
    {
        if (gs.IsTerminal() && !_Terminal(io_next_index, i_parsed_string, gs))
        {
            res = false;
            break;
        }
        else if (gs.IsNonTerminal() && !_TryAllProductions(io_next_index, o_productions_used, i_parsed_string, m_grammar.GetProduction(gs)))
        {
            res = false;
            break;
        }
        else if (gs.IsLambda())
        {
            if (io_next_index == i_parsed_string.size())
                break;
        }
    }

    return res;
}

//------------------------------------------------------------------------------
bool SyntaxAnalysis::SyntaxAnalyzer::_TryAllProductions(size_t& io_next_index, std::vector<size_t>& o_productions_used, const TSyntaxAnalyzerInput& i_parsed_string, const TProductionRHS& i_prod_rhs)
{
    size_t next_index_cached = io_next_index;
    std::vector<size_t> productions_cached = o_productions_used;

    for (size_t i = 0; i < i_prod_rhs.size(); ++i)
    {
        io_next_index = next_index_cached;
        o_productions_used = productions_cached;
        o_productions_used.push_back(i);
        if (_TryProduction(io_next_index, o_productions_used, i_parsed_string, i_prod_rhs[i]))
        {
            return true;
        }
    }
    return false;
}



