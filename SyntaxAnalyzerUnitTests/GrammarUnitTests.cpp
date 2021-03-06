#include "stdafx.h"
#include "CppUnitTest.h"

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

#include <SyntaxAnalyzer\GrammarSymbol.h>
#include <SyntaxAnalyzer\Grammar.h>

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SyntaxAnalysis;

namespace SyntaxAnalyzerUnitTests
{
	TEST_CLASS(GrammarUnitTests)
	{
	public:
		
		TEST_METHOD(ShouldCreateGrammarSymbol)
		{
            GrammarSymbol lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);
            GrammarSymbol term_symbol = Terminal("terminal");
            GrammarSymbol nonterm_symbol = NonTerminal("nonterm");

            Assert::IsTrue(GrammarSymbol::GST_LAMBDA == lambda_symbol.GetType());
            Assert::IsTrue(GrammarSymbol::GST_TERMINAL == term_symbol.GetType());
            Assert::IsTrue(GrammarSymbol::GST_NONTERMINAL == nonterm_symbol.GetType());
		}


        TEST_METHOD(ShouldCreateGrammar)
        {
            GrammarSymbol lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);
            GrammarSymbol term_symbol = Terminal("terminal");
            GrammarSymbol nonterm_symbol = NonTerminal("nonterm");

            Grammar g(nonterm_symbol);
            g(nonterm_symbol, { { Terminal("+") }, { Terminal("-") } });

            Assert::IsTrue(nonterm_symbol == g.GetStartSymbol());
            Assert::IsTrue(2 == g.GetProduction(nonterm_symbol).size());

        }
	};
}