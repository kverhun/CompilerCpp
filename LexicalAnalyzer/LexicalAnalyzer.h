#pragma once

#include "_LexicalAnalyzerLib.h"

#include <vector>
#include <string>

#include "ILanguageInfo.h"
#include "DFA.h"

namespace LexicalAnalysis
{
	LEXICALANALYZER_API class LexicalAnalyzer
	{
	public:
		
		typedef std::vector<ILanguageInfo::TLexemeClass> TParsedString;

	public:
		LexicalAnalyzer(const ILanguageInfo& i_lang_info);

		TParsedString ParseString(const std::string& i_string);


    private:

        // returns false if noone accepted
        bool _UpdateAutomatsState(DFA::TInput i_input);

        void _ResetAutomats() const;

	private:
		const ILanguageInfo& m_lang_info;

        std::vector<ILanguageInfo::TLexemeClass> m_should_check;
        ILanguageInfo::TLexemeClass m_last_best_fit;
        size_t m_current_start_index;
        size_t m_best_fit_lenght;
        

        size_t m_current_index;

	};

}