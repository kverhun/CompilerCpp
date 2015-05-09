#pragma once

#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include "DFA.h"

namespace LexicalAnalysis
{
	class LanguageInfoCpp : public ILanguageInfo
	{
	public:
		enum ELexemeClasses : size_t
		{
            LC_KEYWORD,
            LC_IDENTIFIER        = LC_KEYWORD + 1,
            LC_INTEGER_LITERAL   = LC_IDENTIFIER + 1,
            LC_CHARACTER_LITERAL = LC_INTEGER_LITERAL + 1,
            LC_FLOATING_LITERAL  = LC_CHARACTER_LITERAL + 1,
            LC_STRING_LITERAL    = LC_FLOATING_LITERAL + 1,
            LC_BOOLEAN_LITERAL   = LC_STRING_LITERAL + 1,
            LC_POINTER_LITERAL   = LC_BOOLEAN_LITERAL + 1,
            //
            LC_PUNCTUATION_OR_OPERATOR = ILanguageInfo::SEPARATOR_LEXEME,
            LC_ERROR = ILanguageInfo::ERROR_LEXEME
		};

	public:
	
		LanguageInfoCpp();

		virtual ~LanguageInfoCpp();

		virtual const TLexemeAutomataPoolMap& GetLexemeAutomataPoolMap() const override;

		virtual const TLexemeClassPriority& GetLexemeClassPriority() const override;

		virtual const TLexemeClasses& GetLexemeClasses() const override;
	
		virtual const TAlphabet& GetAlphabet() const override;

        virtual bool IsCharSeparator(char i_char) const override;

	private:
		void _CreateAlphabet();
		void _CreateLexemeClasses();
		void _CreateLexemeAutomataPoolMap();

	private:
		TAlphabet m_alphabet;
		TLexemeClasses m_lexeme_classes;
		TLexemeClassPriority m_lexeme_class_priority;
		TLexemeAutomataPoolMap m_lexeme_automata_pool_map;

        DFA::TInputSet m_letters;
        DFA::TInputSet m_digits;
        DFA::TInputSet m_nonzero_digits;
        DFA::TInputSet m_separators;
        DFA::TInputSet m_octal_digits;
        DFA::TInputSet m_hexadecimal_digits;
        DFA::TInputSet m_c_char;
        DFA::TInputSet m_escape_char;
	};

}