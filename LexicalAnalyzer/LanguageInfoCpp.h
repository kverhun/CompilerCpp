#pragma once

#include "ILanguageInfo.h"
#include "DFA.h"

namespace LexicalAnalysis
{
	class LanguageInfoCpp : public ILanguageInfo
	{
	public:
		enum ELexemeClasses
		{
            LC_KEYWORD = 1,
            LC_IDENTIFIER = 2,
            //LC_SEPARATOR = 3,
            LC_PUNCTUATION_OR_OPERATOR = ILanguageInfo::SEPARATOR_LEXEME,
            LC_INTEGER_LITERAL = 5,
            LC_CHARACTER_LITERAL = 6,
            LC_FLOATING_LITERAL = 7,
            LC_STRING_LITERAL = 8,
            LC_BOOLEAN_LITERAL = 9,
            LC_POINTER_LITERAL = 10,

            //
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