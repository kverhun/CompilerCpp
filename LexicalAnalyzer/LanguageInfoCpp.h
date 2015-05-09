#pragma once

#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include <Interfaces\LexicalAnalyzer\CppLexemeClasses.h>
#include "DFA.h"

namespace LexicalAnalysis
{
	class LanguageInfoCpp : public ILanguageInfo
	{
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