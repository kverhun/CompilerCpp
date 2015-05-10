#pragma once

#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <boost\optional.hpp>

namespace SyntaxAnalysis
{
    class GrammarSymbol
    {
    public:
        enum EGrammarSymbolType
        {
            GST_TERMINAL,
            GST_NONTERMINAL,
            GST_LAMBDA
        };

    public:
        GrammarSymbol(EGrammarSymbolType i_type);
        EGrammarSymbolType GetType() const;

        LexicalAnalysis::LexemeInfo GetTerminalInfo() const;
        const std::string&          GetNonterminalInfo() const;
        bool IsLambda() const;

        // for usage in std containers
        bool operator < (const GrammarSymbol& i_other) const;

    private:
        EGrammarSymbolType m_type;

        boost::optional<LexicalAnalysis::LexemeInfo> m_terminal_info;
        boost::optional<std::string>                 m_nonterminal_info;
    };
}