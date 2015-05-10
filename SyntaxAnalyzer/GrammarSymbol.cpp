#include "GrammarSymbol.h"

#include <assert.h>

using namespace SyntaxAnalysis;

//------------------------------------------------------------------------------
GrammarSymbol::GrammarSymbol(EGrammarSymbolType i_type)
: m_type(i_type)
{

}

//------------------------------------------------------------------------------
GrammarSymbol::GrammarSymbol(LexicalAnalysis::LexemeInfo i_term_info)
: m_type(GST_TERMINAL)
, m_terminal_info(i_term_info)
{

}

//------------------------------------------------------------------------------
GrammarSymbol::GrammarSymbol(const std::string& i_nonterm_info)
: m_type(GST_NONTERMINAL)
, m_nonterminal_info(i_nonterm_info)
{

}

//------------------------------------------------------------------------------
GrammarSymbol::GrammarSymbol(const GrammarSymbol& i_other)
: m_type(i_other.m_type)
, m_terminal_info(i_other.m_terminal_info)
, m_nonterminal_info(i_other.m_nonterminal_info)
{

}

//------------------------------------------------------------------------------
GrammarSymbol::EGrammarSymbolType GrammarSymbol::GetType() const
{
    return m_type;
}

//------------------------------------------------------------------------------
LexicalAnalysis::LexemeInfo GrammarSymbol::GetTerminalInfo() const
{
    assert(m_terminal_info);
    return *m_terminal_info;
}

//------------------------------------------------------------------------------
const std::string& GrammarSymbol::GetNonterminalInfo() const
{
    assert(m_nonterminal_info);
    return *m_nonterminal_info;
}

//------------------------------------------------------------------------------
bool GrammarSymbol::IsLambda() const
{
    return m_type == GST_LAMBDA;
}

//------------------------------------------------------------------------------
bool GrammarSymbol::operator<(const GrammarSymbol& i_other) const
{
    if (m_type != i_other.m_type)
        return m_type < i_other.m_type;

    switch (m_type)
    {
    case GST_TERMINAL:
        return m_terminal_info->m_lexeme_value < i_other.m_terminal_info->m_lexeme_value;
    case GST_NONTERMINAL:
        return m_nonterminal_info < i_other.m_nonterminal_info;
    case GST_LAMBDA:
        return false;
    default:
        throw std::logic_error("Invalid Grammar symbol type");
    }
}

//------------------------------------------------------------------------------
bool GrammarSymbol::operator==(const GrammarSymbol& i_other) const
{
    return !(*this < i_other) && !(i_other < *this);
}

//------------------------------------------------------------------------------
bool GrammarSymbol::IsTerminal() const
{
    return m_type == GrammarSymbol::GST_TERMINAL;
}

//------------------------------------------------------------------------------
bool GrammarSymbol::IsNonTerminal() const
{
    return m_type == GrammarSymbol::GST_NONTERMINAL;
}
