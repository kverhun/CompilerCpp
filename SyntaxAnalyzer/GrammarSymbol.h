#pragma once

#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <boost\optional.hpp>

namespace SyntaxAnalysis
{
    class Terminal
    {
        friend class GrammarSymbol;
        friend bool operator == (const Terminal& i_terminal, const LexicalAnalysis::LexemeInfo& i_lexeme_info);
        friend bool operator == (const LexicalAnalysis::LexemeInfo& i_lexeme_info, const Terminal& i_terminal);
    public:
        Terminal(const std::string& i_str, size_t i_idx = size_t(-1));
        Terminal(const Terminal& i_other);
        bool operator == (const Terminal& i_other) const;
        bool operator < (const Terminal& i_other) const;
        size_t GetIndex() const;
        void SetIndex(size_t i_idx);
    private:
        std::string m_str;
        size_t m_idx;
    };

    bool operator == (const Terminal& i_terminal, const LexicalAnalysis::LexemeInfo& i_lexeme_info);
    bool operator == (const LexicalAnalysis::LexemeInfo& i_lexeme_info, const Terminal& i_terminal);

    class NonTerminal
    {
        friend class GrammarSymbol;
    public:
        NonTerminal(const std::string& i_str);
        bool operator == (const NonTerminal& i_other) const;
        bool operator < (const NonTerminal& i_other) const;
    private:
        std::string m_str;
    };

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
        GrammarSymbol(Terminal i_terminal);
        GrammarSymbol(NonTerminal i_nonterminal);
        GrammarSymbol(const GrammarSymbol& i_other);

        EGrammarSymbolType GetType() const;

        const Terminal& GetTerminalInfo() const;
        Terminal& GetTerminalInfo();
        NonTerminal GetNonterminalInfo() const;
        bool IsLambda() const;
        bool IsTerminal() const;
        bool IsNonTerminal() const;

        // for usage in std containers
        bool operator < (const GrammarSymbol& i_other) const;

        bool operator == (const GrammarSymbol& i_other) const;

        std::string ToString() const;

    private:
        EGrammarSymbolType m_type;

        boost::optional<Terminal>    m_terminal_info;
        boost::optional<NonTerminal> m_nonterminal_info;
    };
}