#pragma once

#include "Grammar.h"

namespace SyntaxAnalysis
{
    class GrammarDefinitionHelpers
    {
    public:
        static TProductionLHS ReadProductionLHS(const std::string& i_str);
    };
}