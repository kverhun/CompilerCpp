#include "GrammarDefinitionHelpers.h"

using namespace SyntaxAnalysis;

//------------------------------------------------------------------------------
TProductionLHS GrammarDefinitionHelpers::ReadProductionLHS(const std::string& i_str)
{
    if (i_str == "lambda")
        return GrammarSymbol(GrammarSymbol::GST_LAMBDA);
    else if (i_str[0] == '\'' && i_str[i_str.size() - 1] == '\'')
        return Terminal(i_str.substr(1, i_str.size() - 2));
    else
        return NonTerminal(i_str);
}
