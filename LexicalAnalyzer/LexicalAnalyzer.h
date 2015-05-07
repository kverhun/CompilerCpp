#pragma once

#include "_LexicalAnalyzerLib.h"

#include <vector>
#include <string>

#include "../Interfaces/LexicalAnalyzer/ILanguageInfo.h"
#include "DFA.h"

namespace LexicalAnalysis
{
	LEXICALANALYZER_API class LexicalAnalyzer
	{
	public:
        struct LexemeInfo
        {
            ILanguageInfo::TLexemeClass m_lexeme_class;
            std::string m_lexeme_value;
            size_t m_lexeme_position;
        };

		typedef std::vector<LexemeInfo> TParsedString;

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