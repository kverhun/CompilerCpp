#include "SyntaxAnalysisHelpers.h"

#include "GrammarSymbol.h"

#include <Interfaces\LexicalAnalyzer\CppLexemeClasses.h>

using namespace SyntaxAnalysis;

typedef LexicalAnalysis::TParsedString TParsedString;

TSyntaxAnalyzerInput SyntaxAnalysisHelpers::FixParsedStringForCpp(const TParsedString& i_parsed_string)
{
    TSyntaxAnalyzerInput result;
    result.reserve(i_parsed_string.size());

    for (auto& lexeme : i_parsed_string)
    {
        if (lexeme.m_lexeme_class == LexicalAnalysis::ECppLexemeClasses::LC_IDENTIFIER)
            result.push_back(Terminal("ID"));
        else
            result.push_back(Terminal(lexeme.m_lexeme_value));
    }

    return result;
}
