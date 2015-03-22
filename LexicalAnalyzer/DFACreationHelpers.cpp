#include "DFACreationHelpers.h"

using namespace LexicalAnalysis;

//------------------------------------------------------------------------------
DFA DFACreationHelpers::CreateForString(const std::string& i_string)
{
	DFA dfa;

	dfa.AddState(0, DFA::ST_INTERMEDIATE);

	for (size_t i = 0; i < i_string.size() - 1; ++i)
	{
		dfa.AddState(i+1, DFA::ST_INTERMEDIATE);
		dfa.AddTransition(i, i_string[i], i + 1);
	}
	dfa.AddState(i_string.size(), DFA::ST_TERMINAL_ACCEPTED);
	dfa.AddTransition(i_string.size() - 1, i_string[i_string.size() - 1], i_string.size());

	return dfa;
}

//------------------------------------------------------------------------------
DFA DFACreationHelpers::CreateAccepingAll()
{
    DFA dfa;
    dfa.m_state_map[DFA::mg_error_state] = DFA::ST_TERMINAL_ACCEPTED;
    return dfa;
}
