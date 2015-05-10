#include "SyntaxAnalyzer.h"
#include "Grammar.h"

#include <Interfaces/LexicalAnalyzer/LexemeInfo.h>

#include <functional>

using namespace SyntaxAnalysis;

//------------------------------------------------------------------------------
SyntaxAnalyzer::SyntaxAnalyzer(const Grammar& i_grammar)
: m_grammar(i_grammar)
{

}

//------------------------------------------------------------------------------
bool SyntaxAnalyzer::Analyze(const LexicalAnalysis::TParsedString& i_parsed_string)
{
    return false;
}
