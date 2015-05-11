#pragma once

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

namespace SyntaxAnalysis
{
    struct SyntaxAnalysisHelpers
    {
        static LexicalAnalysis::TParsedString FixParsedStringForCpp(const LexicalAnalysis::TParsedString& i_parsed_string);
    };
}