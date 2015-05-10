#pragma once

#include "Grammar.h"

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

namespace SyntaxAnalysis
{
    class Grammar;

    class SyntaxAnalyzer
    {
    public:
        SyntaxAnalyzer(const Grammar& i_grammar);
        bool Analyze(const LexicalAnalysis::TParsedString& i_parsed_string);

    private:
        bool _TryProduction(size_t& io_next_index, const LexicalAnalysis::TParsedString& i_parsed_string, const TGrammarSymbolSequence& i_symbol_seq);
        bool _TryAllProductions(size_t& io_next_index, const LexicalAnalysis::TParsedString& i_parsed_string, const TProductionRHS& i_prod_rhs);
    private:

        const Grammar& m_grammar;

    };
}

