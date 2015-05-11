#include "SyntaxAnalysisHelpers.h"

#include <Interfaces\LexicalAnalyzer\CppLexemeClasses.h>

using namespace SyntaxAnalysis;

typedef LexicalAnalysis::TParsedString TParsedString;

TParsedString SyntaxAnalysisHelpers::FixParsedStringForCpp(const TParsedString& i_parsed_string)
{
    TParsedString parsed_string = i_parsed_string;

    for (auto& lexeme : parsed_string)
    {
        if (lexeme.m_lexeme_class == LexicalAnalysis::ECppLexemeClasses::LC_IDENTIFIER)
            lexeme.m_lexeme_value = "ID";
    }

    return parsed_string;
}
