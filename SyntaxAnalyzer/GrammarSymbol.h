#pragma once

#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <boost\optional.hpp>

namespace SyntaxAnalysis
{
    class Terminal
    {
        friend bool operator == (const Terminal& i_terminal, const LexicalAnalysis::LexemeInfo& i_lexeme_info);
        friend bool operator == (const LexicalAnalysis::LexemeInfo& i_lexeme_info, const Terminal& i_terminal);
    public:
        Terminal(const std::string& i_str);
        bool operator < (const Terminal& i_other) const;
    private:
        std::string m_str;
    };

    bool operator == (const Terminal& i_terminal, const LexicalAnalysis::LexemeInfo& i_lexeme_info);
    bool operator == (const LexicalAnalysis::LexemeInfo& i_lexeme_info, const Terminal& i_terminal);


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
        GrammarSymbol(LexicalAnalysis::LexemeInfo i_term_info);
        GrammarSymbol(const std::string& i_nonterm_info);

        GrammarSymbol(const GrammarSymbol& i_other);

        EGrammarSymbolType GetType() const;

        Terminal GetTerminalInfo() const;
        const std::string&          GetNonterminalInfo() const;
        bool IsLambda() const;
        bool IsTerminal() const;
        bool IsNonTerminal() const;

        // for usage in std containers
        bool operator < (const GrammarSymbol& i_other) const;

        bool operator == (const GrammarSymbol& i_other) const;

    private:
        EGrammarSymbolType m_type;

        boost::optional<Terminal>    m_terminal_info;
        boost::optional<std::string> m_nonterminal_info;
    };
}