#pragma once

#include "_LexicalAnalyzerLib.h"
#include <string>

#include "DFA.h"

namespace LexicalAnalysis
{
	class DFACreationHelpers
	{
	public:
		static DFA CreateForString(const std::string& i_string);
        static DFA CreateAccepingAll();
	};
}