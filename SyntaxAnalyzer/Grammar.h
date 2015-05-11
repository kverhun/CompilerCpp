#pragma once

#include "GrammarSymbol.h"

#include <boost\optional.hpp>

namespace SyntaxAnalysis
{
    typedef std::vector<GrammarSymbol>               TGrammarSymbolSequence;
    typedef GrammarSymbol                            TProductionLHS;
    typedef std::vector<TGrammarSymbolSequence>      TProductionRHS;
    typedef std::map<TProductionLHS, TProductionRHS> TProductionSet;

    class Grammar
    {
    public:
        Grammar(GrammarSymbol i_start_symbol);
        Grammar& operator()(const TProductionLHS& i_lhs, const TProductionRHS& i_rhs);

        const TProductionRHS& GetProduction(const TProductionLHS& i_lhs) const;
        const GrammarSymbol& GetStartSymbol() const;
    private:
        
        TProductionSet m_productions;
        GrammarSymbol m_start_symbol;
    };

}