#include "SyntaxAnalyzer.h"

#include <Interfaces/LexicalAnalyzer/LexemeInfo.h>

#include <functional>

using namespace SyntaxAnalysis;

namespace
{
    typedef std::vector<GrammarSymbol> TParserState;

    bool _Terminal(size_t& io_next_index, const LexicalAnalysis::TParsedString& i_parsed_string, const GrammarSymbol& i_grammar_symbol)
    {
        bool res = i_parsed_string[io_next_index].m_lexeme_value == i_grammar_symbol.GetTerminalInfo().m_lexeme_value;
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
bool SyntaxAnalyzer::Analyze(const LexicalAnalysis::TParsedString& i_parsed_string)
{
    size_t current_index = 0;
    
    auto start_symbol = m_grammar.GetStartSymbol();
    auto start_symbol_productions = m_grammar.GetProduction(start_symbol);
    
    return _TryAllProductions(current_index, i_parsed_string, start_symbol_productions);
}

//------------------------------------------------------------------------------
bool SyntaxAnalyzer::_TryProduction(size_t& io_next_index, const LexicalAnalysis::TParsedString& i_parsed_string, const TGrammarSymbolSequence& i_symbol_seq)
{
    bool res = true;
    for (auto gs : i_symbol_seq)
    {
        if (gs.IsTerminal() && !_Terminal(io_next_index, i_parsed_string, gs))
        {
            res = false;
            break;
        }
        else if (gs.IsNonTerminal() && !_TryAllProductions(io_next_index, i_parsed_string, m_grammar.GetProduction(gs)))
        {
            res = false;
            break;
        }
    }

    if (io_next_index != i_parsed_string.size())
        res = false;
    return res;
}

//------------------------------------------------------------------------------
bool SyntaxAnalysis::SyntaxAnalyzer::_TryAllProductions(size_t& io_next_index, const LexicalAnalysis::TParsedString& i_parsed_string, const TProductionRHS& i_prod_rhs)
{
    size_t next_index_cached = io_next_index;

    for (auto symbol_seq : i_prod_rhs)
    {
        io_next_index = next_index_cached;
        if (_TryProduction(io_next_index, i_parsed_string, symbol_seq))
            return true;
    }
    return false;
}
