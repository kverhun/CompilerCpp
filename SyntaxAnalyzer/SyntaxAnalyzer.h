#pragma once

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

namespace SyntaxAnalysis
{
    class SyntaxAnalyzer
    {
    public:
        SyntaxAnalyzer();

        bool Analyze(LexicalAnalysis::TParsedString);

    private:
        bool TERM(const std::string& i_str);

        bool E1();
        bool E2();
        bool E();

        bool T1();
        bool T2();
        bool T3();
        bool T();

        size_t m_next;
        LexicalAnalysis::TParsedString m_parsed_string;
    };
}