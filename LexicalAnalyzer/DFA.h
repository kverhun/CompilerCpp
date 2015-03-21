#pragma once

#include "_LexicalAnalyzerLib.h"

#include <map>

namespace LexicalAnalysis
{

	LEXICALANALYZER_API class DFA
	{
	public:
		enum EStateType
		{
			ST_TERMINAL_ACCEPTED,
			ST_TERMINAL_REJECTED,
			ST_INTERMEDIATE
		};

		typedef size_t TState;
		typedef char   TInput;

	public:

		DFA();

		void AddState(TState i_state, EStateType i_type);
		void AddTransition(TState i_from, TInput i_input, TState i_to);
		void SetStartState(TState i_state);

		// reset DFA to initial state
		void Reset() const;

		// reset DFA, process string, reset DFA
		EStateType Process(const std::string& i_string) const;

		// make 1 transition
		EStateType AddInput(TInput i_input) const;

		EStateType GetCurrentState() const;

	private:
		typedef std::pair<TState, TInput> TTransitionMapKey;

		static const TState mg_error_state = TState(-1);

	private:

		std::map<TTransitionMapKey, TState> m_transition_map;
		std::map<TState, EStateType>		m_state_map;
		mutable TState						m_current_state;

		TState	m_start_state;
	};

}