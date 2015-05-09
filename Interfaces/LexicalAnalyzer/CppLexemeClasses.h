#pragma once

#include "ILanguageInfo.h"

namespace LexicalAnalysis
{
    enum ECppLexemeClasses : ILanguageInfo::TLexemeClass
    {
        LC_KEYWORD,
        LC_IDENTIFIER = LC_KEYWORD + 1,
        LC_INTEGER_LITERAL = LC_IDENTIFIER + 1,
        LC_CHARACTER_LITERAL = LC_INTEGER_LITERAL + 1,
        LC_FLOATING_LITERAL = LC_CHARACTER_LITERAL + 1,
        LC_STRING_LITERAL = LC_FLOATING_LITERAL + 1,
        LC_BOOLEAN_LITERAL = LC_STRING_LITERAL + 1,
        LC_POINTER_LITERAL = LC_BOOLEAN_LITERAL + 1,
        //
        LC_PUNCTUATION_OR_OPERATOR = ILanguageInfo::SEPARATOR_LEXEME,
        LC_ERROR = ILanguageInfo::ERROR_LEXEME
    };
}