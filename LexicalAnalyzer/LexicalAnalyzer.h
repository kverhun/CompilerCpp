#pragma once

#include "_LexicalAnalyzerLib.h"

#include <vector>
#include <string>

#include "ILanguageInfo.h"

namespace LexicalAnalysis
{
	class DFA;

	LEXICALANALYZER_API class LexicalAnalyzer
	{
	public:
		
		typedef std::vector<ILanguageInfo::TLexemeClass> TParsedString;

	public:
		LexicalAnalyzer(const ILanguageInfo& i_lang_info);

		TParsedString ParseString(const std::string& i_string) const;

	private:

	private:
		const ILanguageInfo& m_lang_info;

	};

}