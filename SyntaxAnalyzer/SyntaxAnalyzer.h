#pragma once

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

        const Grammar& m_grammar;

    };
}

