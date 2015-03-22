#include "DFA.h"


using namespace LexicalAnalysis;

//------------------------------------------------------------------------------
DFA::DFA()
{
	m_state_map[mg_error_state] = ST_TERMINAL_REJECTED;
}

//------------------------------------------------------------------------------
DFA::DFA(const DFA& i_other)
{
    m_current_state = i_other.m_current_state;
    m_start_state = i_other.m_start_state;
    m_state_map = i_other.m_state_map;
    m_transition_map = i_other.m_transition_map;
}

//------------------------------------------------------------------------------
void DFA::AddState(TState i_state, EStateType i_type)
{
	if (m_state_map.find(i_state) != m_state_map.end())
	{
		if (m_state_map[i_state] == i_type)
			return;
		else
			throw std::logic_error("duplicate state with different types");
	}
	else
		m_state_map[i_state] = i_type;
}

//------------------------------------------------------------------------------
void DFA::AddTransition(TState i_from, TInput i_input, TState i_to)
{
	auto key = std::make_pair(i_from, i_input);
	if (m_transition_map.find(key) != m_transition_map.end())
	{
		if (m_transition_map[key] == i_to)
			return;
		else
			throw std::logic_error("nondeterministic transition possible");
	}
	else
		m_transition_map[key] = i_to;
}

//------------------------------------------------------------------------------
void LexicalAnalysis::DFA::AddTransition(TState i_from, const TInputSet& i_input, TState i_to)
{
    for (auto inp : i_input)
        AddTransition(i_from, inp, i_to);
}

//------------------------------------------------------------------------------
DFA& LexicalAnalysis::DFA::operator()(TState i_state, EStateType i_type)
{
    AddState(i_state, i_type);
    return *this;
}

//------------------------------------------------------------------------------
DFA& LexicalAnalysis::DFA::operator()(TState i_from, TInput i_input, TState i_to)
{
    AddTransition(i_from, i_input, i_to);
    return *this;
}

//------------------------------------------------------------------------------
DFA& LexicalAnalysis::DFA::operator()(TState i_from, TInputSet i_input, TState i_to)
{
    AddTransition(i_from, i_input, i_to);
    return *this;
}


//------------------------------------------------------------------------------
void DFA::Reset() const
{
	m_current_state = m_start_state;
}

//------------------------------------------------------------------------------
DFA::EStateType DFA::Process(const std::string& i_string) const
{
	Reset();
	for (auto ch : i_string)
		AddInput(ch);
	
	auto result = m_state_map.find(m_current_state)->second;
	Reset();
	return result;
}

//------------------------------------------------------------------------------
DFA::EStateType DFA::AddInput(TInput i_input) const
{
	auto transition_key = std::make_pair(m_current_state, i_input);
	auto new_state_iter = m_transition_map.find(transition_key);
	
	if (new_state_iter != m_transition_map.end())
	{
		m_current_state = new_state_iter->second;
		return m_state_map.find(m_current_state)->second;
	}
	else
	{
		m_current_state = mg_error_state;
		return m_state_map.find(m_current_state)->second;
	}
}

//------------------------------------------------------------------------------
void DFA::SetStartState(TState i_state)
{
	if (m_state_map.find(i_state) != m_state_map.end())
		m_start_state = i_state;
	else
		throw std::logic_error("Setting unknown state as start");
}

//------------------------------------------------------------------------------
DFA::EStateType DFA::GetCurrentState() const
{
	return m_state_map.find(m_current_state)->second;
}
