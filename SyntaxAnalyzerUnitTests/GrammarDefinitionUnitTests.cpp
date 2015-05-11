#include "stdafx.h"
#include "CppUnitTest.h"

#include <SyntaxAnalyzer/GrammarDefinitionHelpers.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SyntaxAnalysis;
namespace SyntaxAnalyzerUnitTests
{
	TEST_CLASS(GrammarDefinitionUnitTests)
	{
	public:
		
		TEST_METHOD(ShouldCreateProductionLHS)
		{
            auto terminal_str = "\'if\'";
            auto nonterminal_str = "expression";
            auto lambda_str = "lambda";

            auto terminal_expected = Terminal("if");
            auto nonterminal_expected = NonTerminal("expression");

            auto terminal_lhs = GrammarDefinitionHelpers::ReadProductionLHS(terminal_str);
            auto nonterminal_lhs = GrammarDefinitionHelpers::ReadProductionLHS(nonterminal_str);
            auto lambda_lhs = GrammarDefinitionHelpers::ReadProductionLHS(lambda_str);

            Assert::IsTrue(terminal_lhs.IsTerminal());
            Assert::IsTrue(terminal_expected == terminal_lhs.GetTerminalInfo());
            
            Assert::IsTrue(nonterminal_lhs.IsNonTerminal());
            Assert::IsTrue(nonterminal_expected == nonterminal_lhs.GetNonterminalInfo());

            Assert::IsTrue(lambda_lhs.IsLambda());

		}

	};
}