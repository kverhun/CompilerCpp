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
        bool should_continue = _UpdateAutomatsState(i_string[i]);
        if (!should_continue)
        {
            if (isspace(i_string[i]))
            {
                _ResetAutomats();

                if (m_best_fit_lenght != 0 && m_current_start_index + m_best_fit_lenght == i)
                {
                    std::string parsed_lexeme = i_string.substr(m_current_start_index, m_best_fit_lenght);
                    parsed_string.push_back({ m_last_best_fit, parsed_lexeme, m_current_start_index });
                }
                else if (i - m_current_start_index > 0)
                {
                    std::string parsed_lexeme = i_string.substr(m_current_start_index, i - m_current_start_index);
                    parsed_string.push_back({ ILanguageInfo::ERROR_LEXEME, parsed_lexeme, m_current_start_index });
                }

                ++i;

                m_current_start_index = i;
                m_best_fit_lenght = 0;
                continue;
            }
            else if (m_last_best_fit == ILanguageInfo::SEPARATOR_LEXEME && m_current_start_index + m_best_fit_lenght == i - 1)
            {
                _ResetAutomats();

                if (m_best_fit_lenght > 0)
                {
                    std::string parsed_lexeme = i_string.substr(m_current_start_index, m_best_fit_lenght);
                    parsed_string.push_back({ m_last_best_fit, parsed_lexeme, m_current_start_index });
                }
                else if (i - m_current_start_index > 0)
                {
                    std::string parsed_lexeme = i_string.substr(m_current_start_index, i - m_current_start_index);
                    parsed_string.push_back({ ILanguageInfo::ERROR_LEXEME, parsed_lexeme, m_current_start_index });
                }

                --i;

                m_current_start_index = i;
                m_best_fit_lenght = 0;
                continue;

            }
            else
            {
                if (m_lang_info.IsCharSeparator(i_string[i]))
                {
                    _ResetAutomats();

                    if (m_best_fit_lenght != 0 && m_current_start_index + m_best_fit_lenght == i)
                    {
                        std::string parsed_lexeme = i_string.substr(m_current_start_index, m_best_fit_lenght);
                        parsed_string.push_back({ m_last_best_fit, parsed_lexeme, m_current_start_index });
                    }
                    else if (i - m_current_start_index > 0)
                    {
                        std::string parsed_lexeme = i_string.substr(m_current_start_index, i - m_current_start_index);
                        parsed_string.push_back({ ILanguageInfo::ERROR_LEXEME, parsed_lexeme, m_current_start_index });
                    }

                    m_current_start_index = i;
                    m_best_fit_lenght = 0;
                    continue;
                }
                else
                {
                    ++i;
                    continue;
                }
            }

            if (m_best_fit_lenght != 0)
            {
                std::string parsed_lexeme = i_string.substr(m_current_start_index, m_best_fit_lenght);
                parsed_string.push_back({ m_last_best_fit, parsed_lexeme, m_current_start_index });
                m_current_start_index += m_best_fit_lenght;
                m_best_fit_lenght = 0;
                //m_current_start_index = i;
                _ResetAutomats();
                continue;
            }
        }
        ++i;
    }

    if (m_best_fit_lenght != 0 && m_current_start_index + m_best_fit_lenght == i)
    {
        std::string parsed_lexeme = i_string.substr(m_current_start_index, m_best_fit_lenght);
        parsed_string.push_back({ m_last_best_fit, parsed_lexeme, m_current_start_index });
    }
    else if (i - m_current_start_index > 0)
    {
        std::string parsed_lexeme = i_string.substr(m_current_start_index, i - m_current_start_index);
        parsed_string.push_back({ ILanguageInfo::ERROR_LEXEME, parsed_lexeme, m_current_start_index });
    }

	return parsed_string;
}

//------------------------------------------------------------------------------
bool LexicalAnalyzer::_UpdateAutomatsState(DFA::TInput i_input)
{
    bool any_has_chance = false;
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
            if (lexeme_type != ILanguageInfo::ERROR_LEXEME && state != DFA::ST_TERMINAL_REJECTED)
            {
                any_has_chance = true;
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
    return any_has_chance;
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
