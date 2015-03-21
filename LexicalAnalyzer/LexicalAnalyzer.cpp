#include "LexicalAnalyzer.h"

using namespace LexicalAnalysis;

//------------------------------------------------------------------------------
LexicalAnalyzer::LexicalAnalyzer(const ILanguageInfo& i_lang_info)
: m_lang_info(i_lang_info)
{

}

//------------------------------------------------------------------------------
LexicalAnalyzer::TParsedString LexicalAnalyzer::ParseString(const std::string& i_string) const
{
	TParsedString parsed_string;
	
	return parsed_string;
}
