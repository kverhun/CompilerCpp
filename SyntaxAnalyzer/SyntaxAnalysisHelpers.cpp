#include "SyntaxAnalysisHelpers.h"

#include "GrammarSymbol.h"

#include <Interfaces\LexicalAnalyzer\CppLexemeClasses.h>

#include <algorithm>

using namespace SyntaxAnalysis;

typedef LexicalAnalysis::TParsedString TParsedString;
typedef SyntaxAnalysis::TGrammarSymbolSequence TGrammarSymbolSequence;

//------------------------------------------------------------------------------
TSyntaxAnalyzerInput SyntaxAnalysisHelpers::FixParsedStringForCpp(const TParsedString& i_parsed_string)
{
    TSyntaxAnalyzerInput result;
    result.reserve(i_parsed_string.size());

    for (auto& lexeme : i_parsed_string)
    {
        if (lexeme.m_lexeme_class == LexicalAnalysis::ECppLexemeClasses::LC_IDENTIFIER)
            result.push_back(Terminal("ID"));
        else
            result.push_back(Terminal(lexeme.m_lexeme_value));
    }

    return result;
}

//------------------------------------------------------------------------------
std::vector<SyntaxAnalysis::TGrammarSymbolSequence> SyntaxAnalysisHelpers::GetDerivation(const SyntaxAnalysis::Grammar& i_grammar, const std::vector<size_t>& i_productions_used)
{
    std::vector<TGrammarSymbolSequence> result;
    result.reserve(i_productions_used.size() + 1);
    result.push_back({ GrammarSymbol(i_grammar.GetStartSymbol()) });

    for (auto prod_id : i_productions_used)
    {
        auto current_symbol_sequence = TGrammarSymbolSequence();

        // [terminals] [nonterminal-to-expand] [rest-of-sequence]
        //  --copy-------------expand---------------copy-------
        auto it = result.rbegin()->begin();
        while (it->IsTerminal())
            ++it;
        current_symbol_sequence.insert(current_symbol_sequence.end(), result.rbegin()->begin(), it);
        
        // assume it exists
        auto current_nonterminal = *it;
        auto current_production = i_grammar.GetProduction(current_nonterminal)[prod_id];
        current_symbol_sequence.insert(current_symbol_sequence.end(), current_production.begin(), current_production.end());

        ++it;
        current_symbol_sequence.insert(current_symbol_sequence.end(), it, result.rbegin()->end());
        
        // remove lambdas
        current_symbol_sequence.erase(std::remove(current_symbol_sequence.begin(), current_symbol_sequence.end(), GrammarSymbol(GrammarSymbol::GST_LAMBDA)), current_symbol_sequence.end());

        result.emplace_back(current_symbol_sequence);
    }

    return result;
}
