#include "stdafx.h"
#include "CppUnitTest.h"

#include <SyntaxAnalyzer\SyntaxAnalyzer.h>
#include <SyntaxAnalyzer\Grammar.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SyntaxAnalysis;
using LexicalAnalysis::LexemeInfo;
using LexicalAnalysis::ILanguageInfo;

namespace SyntaxAnalyzerUnitTests
{		
	TEST_CLASS(SAUnitTests)
	{
	public:
		
        //////////////////////////////////////////////////////////////////////////
        // Test grammar with one production:
        //  S -> A | B | C
        //   S in N
        //   A, B, C in T
        //////////////////////////////////////////////////////////////////////////
		TEST_METHOD(SATest01)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto start_symbol = GrammarSymbol("S");
            Grammar g(start_symbol);

            auto terminal_1 = LexemeInfo({ 0, "A", 0 });
            auto terminal_2 = LexemeInfo({ 0, "B", 0 });
            auto terminal_3 = LexemeInfo({ 0, "C", 0 });

            g(start_symbol, { { GrammarSymbol(terminal_1) }, { GrammarSymbol(terminal_2) }, {GrammarSymbol(terminal_3)} });

            TParsedString str1 = { LexemeInfo{ 0, "A", 0 } };
            TParsedString str2 = { LexemeInfo{ 0, "B", 0 } };
            TParsedString str3 = { LexemeInfo{ 0, "C", 0 } };
                                             
            TParsedString str4 = { LexemeInfo{ 0, "D", 0 } };
            TParsedString str5 = { LexemeInfo{ 0, "AB", 0 } };
            TParsedString str6 = { LexemeInfo{ 0, "A", 0 }, LexemeInfo{ 0, "B", 0 } };

            SyntaxAnalyzer sa(g);
            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));

            Assert::IsFalse(sa.Analyze(str4));
            Assert::IsFalse(sa.Analyze(str5));
            Assert::IsFalse(sa.Analyze(str6));
		}

        //////////////////////////////////////////////////////////////////////////
        // Test Grammar with nonterminal
        // E -> 'INT' | 'INT' '+' E
        //   
        //////////////////////////////////////////////////////////////////////////
        TEST_METHOD(SATest02)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto start_symbol = GrammarSymbol("E");
            Grammar g(start_symbol);

            auto terminal_1 = LexemeInfo{ 0, "INT", 0 };
            auto terminal_2 = LexemeInfo{ 0, "+", 0 };

            g(start_symbol, { { GrammarSymbol(terminal_1), GrammarSymbol(terminal_2), start_symbol }, { GrammarSymbol(terminal_1) } });

            TParsedString str1 = { LexemeInfo{ 0, "INT", 0 } };
            TParsedString str2 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str3 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 } };

            TParsedString str4 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str5 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 } };
            TParsedString str6 = { LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str7 = { LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str8 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 } };
            TParsedString str9 = { LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 } };
            TParsedString str10 = { LexemeInfo{ 0, "+", 0 } };

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));

            Assert::IsFalse(sa.Analyze(str4));
            Assert::IsFalse(sa.Analyze(str5));
            Assert::IsFalse(sa.Analyze(str6));
            Assert::IsFalse(sa.Analyze(str7));
            Assert::IsFalse(sa.Analyze(str8));
            Assert::IsFalse(sa.Analyze(str9));
            Assert::IsFalse(sa.Analyze(str10));
        }

        TEST_METHOD(SAShouldWorkCorrectWithEmptyStrings)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto start_symbol = GrammarSymbol("E");
            Grammar g(start_symbol);

            auto terminal_1 = LexemeInfo{ 0, "INT", 0 };
            auto terminal_2 = LexemeInfo{ 0, "+", 0 };

            g(start_symbol, { { GrammarSymbol(terminal_1) }, { GrammarSymbol(terminal_1), GrammarSymbol(terminal_2), start_symbol } });

            TParsedString empty_str = { };

            SyntaxAnalyzer sa(g);

            Assert::IsFalse(sa.Analyze(empty_str));

        }

        //////////////////////////////////////////////////////////////////////////
        // Test lambda-productions
        //////////////////////////////////////////////////////////////////////////
        TEST_METHOD(SATest03)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto start_symbol = GrammarSymbol("E");
            Grammar g(start_symbol);

            g(start_symbol, { { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });

            TParsedString str1 = {};
            TParsedString str2 = { LexemeInfo{ 0, "E", 0 } };

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsFalse(sa.Analyze(str2));

        }

        TEST_METHOD(SAShouldWorkCorrectWithLambdaProductions)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto start_symbol = GrammarSymbol("E");
            Grammar g(start_symbol);

            auto terminal_1 = LexemeInfo{ 0, "INT", 0 };
            auto terminal_2 = LexemeInfo{ 0, "+", 0 };

            g(start_symbol, { { GrammarSymbol(terminal_1), GrammarSymbol(terminal_2), start_symbol }, { GrammarSymbol(terminal_1) }, { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });

            TParsedString empty_str = {};

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(empty_str));

            TParsedString str1 = { LexemeInfo{ 0, "INT", 0 } };
            TParsedString str2 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str3 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 } };

            TParsedString str4 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str5 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 } };
            TParsedString str6 = { LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str7 = { LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str8 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 } };
            TParsedString str9 = { LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 }, LexemeInfo{ 0, "+", 0 } };
            TParsedString str10 = { LexemeInfo{ 0, "+", 0 } };

            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));

            Assert::IsFalse(sa.Analyze(str4));
            Assert::IsFalse(sa.Analyze(str5));
            Assert::IsFalse(sa.Analyze(str6));
            Assert::IsFalse(sa.Analyze(str7));
            Assert::IsFalse(sa.Analyze(str8));
            Assert::IsFalse(sa.Analyze(str9));
            Assert::IsFalse(sa.Analyze(str10));
        }

        TEST_METHOD(SAShouldWorkWithOptional)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto start_symbol = GrammarSymbol("int-seq-opt");
            Grammar g(start_symbol);

            auto int_seq = GrammarSymbol("int-seq");
            auto terminal_1 = LexemeInfo{ 0, "INT", 0 };

            g(start_symbol, { { int_seq }, { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });
            g(int_seq, { { GrammarSymbol(terminal_1), int_seq }, { GrammarSymbol(terminal_1) } });

            TParsedString str1 = {};
            TParsedString str2 = { LexemeInfo{ 0, "INT", 0 } };
            TParsedString str3 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 } };
            TParsedString str4 = { LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 }, LexemeInfo{ 0, "INT", 0 } };

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));

        }

        TEST_METHOD(SACppTest01)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto assingment_operator = GrammarSymbol("assignment-operator");
            auto assingment_expression = GrammarSymbol("assignment-expression");
            auto logical_or_expression = GrammarSymbol("logical-or-expression");

            auto terminal_lexeme = LexemeInfo{ 0, "ID", 0 };
            auto terminal_or_op_lexeme = LexemeInfo{ 0, "||", 0 };
            auto terminal_ass_op1_lexeme = LexemeInfo{ 0, "=", 0 };
            auto terminal_ass_op2_lexeme = LexemeInfo{ 0, "+=", 0 };

            auto terminal = GrammarSymbol(terminal_lexeme);
            auto terminal_or_op = GrammarSymbol(terminal_or_op_lexeme);
            auto terminal_ass_op1 = GrammarSymbol(terminal_ass_op1_lexeme);
            auto terminal_ass_op2 = GrammarSymbol(LexemeInfo{ terminal_ass_op2_lexeme});

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(assingment_expression);
            auto logical_or_expression_ex = GrammarSymbol("logical-or-expression-ex");

            grammar
                (logical_or_expression, { { terminal, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_or_op, terminal, logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, {logical_or_expression} })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                ;

            TParsedString str2 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme };
            TParsedString str3 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme };
            TParsedString str4 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_or_op_lexeme, terminal_lexeme };

            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));

        }

	};
}