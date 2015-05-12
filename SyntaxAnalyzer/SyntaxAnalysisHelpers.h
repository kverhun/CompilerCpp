#pragma once

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <Interfaces\SyntaxAnalyzer\SyntaxAnalyzerInput.h>
#include <SyntaxAnalyzer\Grammar.h>

namespace SyntaxAnalysis
{
    struct SyntaxAnalysisHelpers
    {
        static SyntaxAnalysis::TSyntaxAnalyzerInput FixParsedStringForCpp(const LexicalAnalysis::TParsedString& i_parsed_string);

        static std::vector<SyntaxAnalysis::TGrammarSymbolSequence> GetDerivation(const SyntaxAnalysis::Grammar& i_grammar, const std::vector<size_t>& i_productions_used);
    };
}