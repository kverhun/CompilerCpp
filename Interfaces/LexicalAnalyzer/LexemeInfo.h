#pragma once

namespace LexicalAnalysis
{
    struct LexemeInfo
    {
        ILanguageInfo::TLexemeClass m_lexeme_class;
        std::string m_lexeme_value;
        size_t m_lexeme_position;
    };

    typedef std::vector<LexemeInfo> TParsedString;
}