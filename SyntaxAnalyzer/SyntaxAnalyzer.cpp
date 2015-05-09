#include "SyntaxAnalyzer.h"

#include <Interfaces/LexicalAnalyzer/LexemeInfo.h>

#include <functional>

using namespace SyntaxAnalysis;

//------------------------------------------------------------------------------
SyntaxAnalyzer::SyntaxAnalyzer()
{

}

//------------------------------------------------------------------------------
bool SyntaxAnalyzer::Analyze(LexicalAnalysis::TParsedString i_parse_str)
{
    m_parsed_string = i_parse_str;
    m_next = 0;
    return E();
}

bool SyntaxAnalyzer::E1()
{
    return T();
}

bool SyntaxAnalyzer::E2()
{
    return T() && TERM("+") && E();
}

bool SyntaxAnalyzer::E()
{
    auto next = m_next;
    return (m_next = next, E1())
        || (m_next = next, E2());
}

bool SyntaxAnalyzer::T1()
{
    return TERM("INT");
}

bool SyntaxAnalyzer::T2()
{
    return TERM("INT") && TERM("*") && T();
}

bool SyntaxAnalyzer::T3()
{
    return TERM("(") && E() && TERM(")");
}

bool SyntaxAnalyzer::T()
{
    auto next = m_next;
    return (m_next = next, T1())
        || (m_next = next, T2())
        || (m_next = next, T3());
}

//------------------------------------------------------------------------------
bool SyntaxAnalysis::SyntaxAnalyzer::TERM(const std::string& i_str)
{
    bool res = m_parsed_string[m_next].m_lexeme_value == i_str;
    ++m_next;
    return res;
}
