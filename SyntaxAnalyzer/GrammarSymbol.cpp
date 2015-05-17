#include "GrammarSymbol.h"

#include <assert.h>

using namespace SyntaxAnalysis;

//------------------------------------------------------------------------------
GrammarSymbol::GrammarSymbol(EGrammarSymbolType i_type)
: m_type(i_type)
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
GrammarSymbol::GrammarSymbol(Terminal i_terminal)
: m_terminal_info(i_terminal)
, m_type(GST_TERMINAL)
{

}

//------------------------------------------------------------------------------
GrammarSymbol::GrammarSymbol(NonTerminal i_nonterminal)
: m_nonterminal_info(i_nonterminal)
, m_type(GST_NONTERMINAL)
{

}

//------------------------------------------------------------------------------
GrammarSymbol::EGrammarSymbolType GrammarSymbol::GetType() const
{
    return m_type;
}

//------------------------------------------------------------------------------
const Terminal& GrammarSymbol::GetTerminalInfo() const
{
    assert(m_terminal_info);
    return *m_terminal_info;
}

//------------------------------------------------------------------------------
Terminal& SyntaxAnalysis::GrammarSymbol::GetTerminalInfo()
{
    assert(m_terminal_info);
    return *m_terminal_info;
}

//------------------------------------------------------------------------------
NonTerminal GrammarSymbol::GetNonterminalInfo() const
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
        return *m_terminal_info < *(i_other.m_terminal_info);
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

//------------------------------------------------------------------------------
std::string GrammarSymbol::ToString() const
{
    if (IsTerminal())
        return m_terminal_info->m_str;
    else if (IsNonTerminal())
        return m_nonterminal_info->m_str;
    else
        return std::string("");
}

//------------------------------------------------------------------------------
Terminal::Terminal(const std::string& i_str, size_t i_idx)
: m_str(i_str)
, m_idx(i_idx)
{

}

//------------------------------------------------------------------------------
bool Terminal::operator<(const Terminal& i_other) const
{
    return m_str < i_other.m_str;
}

//------------------------------------------------------------------------------
bool Terminal::operator==(const Terminal& i_other) const
{
    return m_str == i_other.m_str;
}

//------------------------------------------------------------------------------
size_t SyntaxAnalysis::Terminal::GetIndex() const
{
    return m_idx;
}

//------------------------------------------------------------------------------
void Terminal::SetIndex(size_t i_idx)
{
    m_idx = i_idx;
}

//------------------------------------------------------------------------------
Terminal::Terminal(const Terminal& i_other)
: m_str(i_other.m_str)
, m_idx(i_other.m_idx)
{

}



//------------------------------------------------------------------------------
bool SyntaxAnalysis::operator==(const Terminal& i_terminal, const LexicalAnalysis::LexemeInfo& i_lexeme_info)
{
    return i_terminal.m_str == i_lexeme_info.m_lexeme_value;
}

//------------------------------------------------------------------------------
bool SyntaxAnalysis::operator==(const LexicalAnalysis::LexemeInfo& i_lexeme_info, const Terminal& i_terminal)
{
    return i_terminal == i_lexeme_info;
}

//------------------------------------------------------------------------------
NonTerminal::NonTerminal(const std::string& i_str)
: m_str(i_str)
{

}

//------------------------------------------------------------------------------
bool NonTerminal::operator==(const NonTerminal& i_other) const
{
    return m_str == i_other.m_str;
}

//------------------------------------------------------------------------------
bool NonTerminal::operator<(const NonTerminal& i_other) const
{
    return m_str < i_other.m_str;
}
