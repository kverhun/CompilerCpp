#pragma once

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <Interfaces\SyntaxAnalyzer\SyntaxAnalyzerInput.h>

namespace SyntaxAnalysis
{
    struct SyntaxAnalysisHelpers
    {
        static SyntaxAnalysis::TSyntaxAnalyzerInput FixParsedStringForCpp(const LexicalAnalysis::TParsedString& i_parsed_string);
    };
}