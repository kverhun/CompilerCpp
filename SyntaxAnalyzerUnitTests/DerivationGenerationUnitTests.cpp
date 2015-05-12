#include "stdafx.h"
#include "CppUnitTest.h"

#include <SyntaxAnalyzer\SyntaxAnalysisHelpers.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace SyntaxAnalysis;

namespace SyntaxAnalyzerUnitTests
{
	TEST_CLASS(DerivationGenerationUnitTests)
	{
	public:
		
		TEST_METHOD(ShouldReturnCorrectDerivationTest1)
		{
            auto assingment_operator = NonTerminal("assignment-operator");
            auto assingment_expression = NonTerminal("assignment-expression");
            auto logical_or_expression = NonTerminal("logical-or-expression");

            auto terminal = Terminal("ID");
            auto terminal_or_op = Terminal("||");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);
            auto logical_or_expression_ex = NonTerminal("logical-or-expression-ex");
            Grammar grammar(assingment_expression);
            grammar
                (logical_or_expression, { { terminal, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_or_op, terminal, logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, { logical_or_expression } })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                ;

            std::vector<size_t> productions_used_1 = { 1, 0, 0, 1 };
            std::vector<SyntaxAnalysis::TGrammarSymbolSequence> expected_result_1 = {
                {assingment_expression},
                {logical_or_expression},
                {terminal, logical_or_expression_ex},
                {terminal, terminal_or_op, terminal, logical_or_expression_ex},
                {terminal, terminal_or_op, terminal}
            };

            auto actual_result_1 = SyntaxAnalysisHelpers::GetDerivation(grammar, productions_used_1);
            
            Assert::IsTrue(expected_result_1.size() == actual_result_1.size());
            Assert::IsTrue(expected_result_1[0] == actual_result_1[0]);
            Assert::IsTrue(expected_result_1[1] == actual_result_1[1]);
            Assert::IsTrue(expected_result_1[2] == actual_result_1[2]);
            Assert::IsTrue(expected_result_1[3] == actual_result_1[3]);
            Assert::IsTrue(expected_result_1[4] == actual_result_1[4]);

		}

	};
}