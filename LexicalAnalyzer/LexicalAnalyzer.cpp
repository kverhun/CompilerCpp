#include "LexicalAnalyzer.h"

using namespace LexicalAnalysis;

//------------------------------------------------------------------------------
LexicalAnalyzer::LexicalAnalyzer(const ILanguageInfo& i_lang_info)
: m_lang_info(i_lang_info)
{

}

//------------------------------------------------------------------------------
LexicalAnalyzer::TParsedString LexicalAnalyzer::ParseString(const std::string& i_string)
{
	TParsedString parsed_string;
    
    m_best_fit_lenght = 0;
    m_current_index = 0;
    m_current_start_index = 0;

    size_t i = 0;
    while (i < i_string.size())
    {
        m_current_index = i;
        bool accepted = _UpdateAutomatsState(i_string[i]);
        if (!accepted)
        {
            if (isspace(i_string[i]))
            {
                _ResetAutomats();
                
                if (m_best_fit_lenght > 0)
                    parsed_string.push_back(m_last_best_fit);
                else if (i - m_current_start_index > 0)
                    parsed_string.push_back(ILanguageInfo::ERROR_LEXEME);
                ++i;
                
                m_current_start_index = i;
                m_best_fit_lenght = 0;
                continue;
            }

            if (m_best_fit_lenght != 0)
            {
                parsed_string.push_back(m_last_best_fit);
                m_best_fit_lenght = 0;
                m_current_start_index = i;
                _ResetAutomats();
                continue;
            }
        }
        ++i;
    }

    if (m_best_fit_lenght != 0)
        parsed_string.push_back(m_last_best_fit);
    else if (i - m_current_start_index > 0)
        parsed_string.push_back(ILanguageInfo::ERROR_LEXEME);


	return parsed_string;
}

//------------------------------------------------------------------------------
bool LexicalAnalyzer::_UpdateAutomatsState(DFA::TInput i_input)
{
    bool accepted_any = false;
    for (auto lexeme_type : m_lang_info.GetLexemeClassPriority())
    {
        bool accepted = false;
        for (const auto& automat : m_lang_info.GetLexemeAutomataPoolMap().find(lexeme_type)->second)
        {
            auto state = automat.AddInput(i_input);
            if (state == DFA::ST_TERMINAL_ACCEPTED)
            {
                accepted = true;
            }
        }
        if (accepted)
        {
            if (!accepted_any && lexeme_type != ILanguageInfo::ERROR_LEXEME)
            {
                m_best_fit_lenght = m_current_index - m_current_start_index + 1;
                m_last_best_fit = lexeme_type;
                
                accepted_any = true;
            }
            
        }
    }
    return accepted_any;
}

//------------------------------------------------------------------------------
void LexicalAnalyzer::_ResetAutomats() const
{
    for (auto& p : m_lang_info.GetLexemeAutomataPoolMap())
    {
        for (auto& automat : p.second)
            automat.Reset();
    }
}
