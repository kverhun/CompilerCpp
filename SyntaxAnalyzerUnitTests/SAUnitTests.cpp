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

        TEST_METHOD(SACppTest02)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto assingment_operator = GrammarSymbol("assignment-operator");
            auto assingment_expression = GrammarSymbol("assignment-expression");
            auto logical_or_expression = GrammarSymbol("logical-or-expression");
            auto logical_or_expression_ex = GrammarSymbol("logical-or-expression-ex");
            auto logical_and_expression = GrammarSymbol("logical-and-expression");
            auto logical_and_expression_ex = GrammarSymbol("logical-and-expression-ex");
            auto inclusive_or_expression = GrammarSymbol("inclusive-or-expression");
            auto inclusive_or_expression_ex = GrammarSymbol("inclusive-or-expression-ex");
            auto exclusive_or_expression = GrammarSymbol("exlusive-or-expression");
            auto exclusive_or_expression_ex = GrammarSymbol("exclusive-or-expression-ex");
            auto and_expression = GrammarSymbol("and-expression");
            auto and_expression_ex = GrammarSymbol("and-expression-ex");
            auto equality_expression = GrammarSymbol("equality-expression");
            auto equality_expression_ex = GrammarSymbol("equality-expression-ex");
            auto additive_expression = GrammarSymbol("additive-expression");
            auto additive_expression_ex = GrammarSymbol("additive-expression-ex");
            auto multiplicative_expression = GrammarSymbol("multiplicative-expression");
            auto multiplicative_expression_ex = GrammarSymbol("multiplicative-expression-ex");            

            auto terminal_ass_op1_lexeme = LexemeInfo{ 0, "=", 0 };
            auto terminal_ass_op2_lexeme = LexemeInfo{ 0, "+=", 0 };
            auto terminal_lexeme = LexemeInfo{ 0, "ID", 0 };
            auto terminal_or_op_lexeme = LexemeInfo{ 0, "||", 0 };
            auto terminal_logical_and_op_lexeme = LexemeInfo{ 0, "&&", 0 };
            auto terminal_inclusive_or_op_lexeme = LexemeInfo{ 0, "|", 0 };
            auto terminal_exclusive_or_op_lexeme = LexemeInfo{ 0, "^", 0 };
            auto terminal_bitwise_and_op_lexeme = LexemeInfo{ 0, "&", 0 };
            auto terminal_equality_op1_lexeme = LexemeInfo{ 0, "==", 0 };
            auto terminal_equality_op2_lexeme = LexemeInfo{ 0, "!=", 0 };
            auto terminal_add_op1_lexeme = LexemeInfo{ 0, "+", 0 };
            auto terminal_add_op2_lexeme = LexemeInfo{ 0, "-", 0 };
            auto terminal_mult_op1_lexeme = LexemeInfo{ 0, "*", 0 };
            auto terminal_mult_op2_lexeme = LexemeInfo{ 0, "/", 0 };
            auto terminal_mult_op3_lexeme = LexemeInfo{ 0, "%", 0 };

            auto terminal_ass_op1 = GrammarSymbol(terminal_ass_op1_lexeme);
            auto terminal_ass_op2 = GrammarSymbol(LexemeInfo{ terminal_ass_op2_lexeme });
            auto terminal = GrammarSymbol(terminal_lexeme);
            auto terminal_logical_or_op = GrammarSymbol(terminal_or_op_lexeme);
            auto terminal_logical_and_op = GrammarSymbol(terminal_logical_and_op_lexeme);
            auto terminal_inclusive_or_op = GrammarSymbol(terminal_inclusive_or_op_lexeme);
            auto terminal_exclusive_or_op = GrammarSymbol(terminal_exclusive_or_op_lexeme);
            auto terminal_bitwise_and_op = GrammarSymbol(terminal_bitwise_and_op_lexeme);
            auto terminal_equality_op1 = GrammarSymbol(terminal_equality_op1_lexeme);
            auto terminal_equality_op2 = GrammarSymbol(terminal_equality_op2_lexeme);
            auto terminal_add_op1 = GrammarSymbol(terminal_add_op1_lexeme);
            auto terminal_add_op2 = GrammarSymbol(terminal_add_op2_lexeme);
            auto terminal_mult_op1 = GrammarSymbol(terminal_mult_op1_lexeme);
            auto terminal_mult_op2 = GrammarSymbol(terminal_mult_op2_lexeme);
            auto terminal_mult_op3 = GrammarSymbol(terminal_mult_op3_lexeme);

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(assingment_expression);

            grammar
                
                (multiplicative_expression, { { terminal, multiplicative_expression_ex } })
                (multiplicative_expression_ex, { { terminal_mult_op1, terminal, multiplicative_expression_ex }, { terminal_mult_op2, terminal, multiplicative_expression_ex }, {terminal_mult_op3, terminal, multiplicative_expression_ex}, {lambda_symbol} })

                (additive_expression, { {multiplicative_expression, additive_expression_ex} })
                (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, {terminal_add_op2, terminal, additive_expression_ex}, {lambda_symbol} })

                (equality_expression, { { additive_expression, equality_expression_ex } })
                (equality_expression_ex, { { terminal_equality_op1, additive_expression, equality_expression_ex }, { terminal_ass_op2, terminal, equality_expression_ex }, { lambda_symbol } })

                (and_expression, { { equality_expression, and_expression_ex } })
                (and_expression_ex, { { terminal_bitwise_and_op, equality_expression, and_expression_ex }, {lambda_symbol} })

                (exclusive_or_expression, { { and_expression, exclusive_or_expression_ex } })
                (exclusive_or_expression_ex, { { terminal_exclusive_or_op, and_expression, exclusive_or_expression_ex }, {lambda_symbol} })

                (inclusive_or_expression, { {exclusive_or_expression, inclusive_or_expression_ex} })
                (inclusive_or_expression_ex, { {terminal_inclusive_or_op, exclusive_or_expression, inclusive_or_expression_ex}, {lambda_symbol} })

                (logical_and_expression, { { inclusive_or_expression, logical_and_expression_ex } })
                (logical_and_expression_ex, { { terminal_logical_and_op, inclusive_or_expression, logical_and_expression_ex }, {lambda_symbol} })
                (logical_or_expression, { { logical_and_expression, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_logical_or_op, logical_and_expression , logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, { logical_or_expression } })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                ;

            TParsedString str2 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme };
            TParsedString str3 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme };
            TParsedString str4 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_or_op_lexeme, terminal_lexeme };
            TParsedString str5 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_logical_and_op_lexeme, terminal_lexeme };
            TParsedString str6 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_exclusive_or_op_lexeme, terminal_lexeme };
            TParsedString str7 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme };
            TParsedString str8 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme};
            TParsedString str9 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op2_lexeme, terminal_lexeme };
            TParsedString str10 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op2_lexeme, terminal_lexeme };
            
            
            SyntaxAnalyzer sa(grammar); 
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));
            Assert::IsTrue(sa.Analyze(str5));
            Assert::IsTrue(sa.Analyze(str6));
            Assert::IsTrue(sa.Analyze(str7));
            Assert::IsTrue(sa.Analyze(str8));
            Assert::IsTrue(sa.Analyze(str9));
            Assert::IsTrue(sa.Analyze(str10));
            

            TParsedString inv_str_1 = { terminal_logical_and_op_lexeme };
            TParsedString inv_str_2 = { terminal_lexeme, terminal_add_op1_lexeme };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
        }

        TEST_METHOD(SACppTest03)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto expression = GrammarSymbol("expression");

            auto additive_expression = GrammarSymbol("additive-expression");
            auto additive_expression_ex = GrammarSymbol("additive-expression-ex");
            auto multiplicative_expression = GrammarSymbol("multiplicative-expression");
            auto multiplicative_expression_ex = GrammarSymbol("multiplicative-expression-ex");
            auto primary_expression = GrammarSymbol("primary-expression");

            auto terminal_lexeme = LexemeInfo{ 0, "ID", 0 };
            auto terminal_add_op1_lexeme = LexemeInfo{ 0, "+", 0 };
            auto terminal_add_op2_lexeme = LexemeInfo{ 0, "-", 0 };
            auto terminal_mult_op1_lexeme = LexemeInfo{ 0, "*", 0 };
            auto terminal_mult_op2_lexeme = LexemeInfo{ 0, "/", 0 };
            auto terminal_mult_op3_lexeme = LexemeInfo{ 0, "%", 0 };
            auto terminal_left_paren_lexeme = LexemeInfo{ 0, "(", 0 };
            auto terminal_right_paren_lexeme = LexemeInfo{ 0, ")", 0 };

            auto terminal = GrammarSymbol(terminal_lexeme);
            auto terminal_add_op1 = GrammarSymbol(terminal_add_op1_lexeme);
            auto terminal_add_op2 = GrammarSymbol(terminal_add_op2_lexeme);
            auto terminal_mult_op1 = GrammarSymbol(terminal_mult_op1_lexeme);
            auto terminal_mult_op2 = GrammarSymbol(terminal_mult_op2_lexeme);
            auto terminal_mult_op3 = GrammarSymbol(terminal_mult_op3_lexeme);
            auto terminal_left_paren = GrammarSymbol(terminal_left_paren_lexeme);
            auto terminal_right_paren = GrammarSymbol(terminal_right_paren_lexeme);

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(expression);

            grammar
                (primary_expression, { { terminal }, { terminal_left_paren, expression, terminal_right_paren } })

                (multiplicative_expression, { { primary_expression, multiplicative_expression_ex } })
                (multiplicative_expression_ex, { { terminal_mult_op1, terminal, multiplicative_expression_ex }, { terminal_mult_op2, terminal, multiplicative_expression_ex }, { terminal_mult_op3, terminal, multiplicative_expression_ex }, { lambda_symbol } })

                (additive_expression, { { multiplicative_expression, additive_expression_ex } })
                (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, { terminal_add_op2, multiplicative_expression, additive_expression_ex }, { lambda_symbol } })

                (expression, { { additive_expression } })
                ;

            TParsedString str1 = { terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme };
            TParsedString str2 = { terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme };

            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
        }

        TEST_METHOD(SACppTest04)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto assingment_operator = GrammarSymbol("assignment-operator");
            auto assingment_expression = GrammarSymbol("assignment-expression");
            auto logical_or_expression = GrammarSymbol("logical-or-expression");
            auto logical_or_expression_ex = GrammarSymbol("logical-or-expression-ex");
            auto logical_and_expression = GrammarSymbol("logical-and-expression");
            auto logical_and_expression_ex = GrammarSymbol("logical-and-expression-ex");
            auto inclusive_or_expression = GrammarSymbol("inclusive-or-expression");
            auto inclusive_or_expression_ex = GrammarSymbol("inclusive-or-expression-ex");
            auto exclusive_or_expression = GrammarSymbol("exlusive-or-expression");
            auto exclusive_or_expression_ex = GrammarSymbol("exclusive-or-expression-ex");
            auto and_expression = GrammarSymbol("and-expression");
            auto and_expression_ex = GrammarSymbol("and-expression-ex");
            auto equality_expression = GrammarSymbol("equality-expression");
            auto equality_expression_ex = GrammarSymbol("equality-expression-ex");
            auto additive_expression = GrammarSymbol("additive-expression");
            auto additive_expression_ex = GrammarSymbol("additive-expression-ex");
            auto multiplicative_expression = GrammarSymbol("multiplicative-expression");
            auto multiplicative_expression_ex = GrammarSymbol("multiplicative-expression-ex");
            auto primary_expression = GrammarSymbol("primary-expression");
            auto expression = GrammarSymbol("expression");

            auto terminal_lexeme = LexemeInfo{ 0, "ID", 0 };
            auto terminal_ass_op1_lexeme = LexemeInfo{ 0, "=", 0 };
            auto terminal_ass_op2_lexeme = LexemeInfo{ 0, "+=", 0 };
            auto terminal_or_op_lexeme = LexemeInfo{ 0, "||", 0 };
            auto terminal_logical_and_op_lexeme = LexemeInfo{ 0, "&&", 0 };
            auto terminal_inclusive_or_op_lexeme = LexemeInfo{ 0, "|", 0 };
            auto terminal_exclusive_or_op_lexeme = LexemeInfo{ 0, "^", 0 };
            auto terminal_bitwise_and_op_lexeme = LexemeInfo{ 0, "&", 0 };
            auto terminal_equality_op1_lexeme = LexemeInfo{ 0, "==", 0 };
            auto terminal_equality_op2_lexeme = LexemeInfo{ 0, "!=", 0 };
            auto terminal_add_op1_lexeme = LexemeInfo{ 0, "+", 0 };
            auto terminal_add_op2_lexeme = LexemeInfo{ 0, "-", 0 };
            auto terminal_mult_op1_lexeme = LexemeInfo{ 0, "*", 0 };
            auto terminal_mult_op2_lexeme = LexemeInfo{ 0, "/", 0 };
            auto terminal_mult_op3_lexeme = LexemeInfo{ 0, "%", 0 };
            auto terminal_left_paren_lexeme = LexemeInfo{ 0, "(", 0 };
            auto terminal_right_paren_lexeme = LexemeInfo{ 0, ")", 0 };

            auto terminal = GrammarSymbol(terminal_lexeme);
            auto terminal_ass_op1 = GrammarSymbol(terminal_ass_op1_lexeme);
            auto terminal_ass_op2 = GrammarSymbol(LexemeInfo{ terminal_ass_op2_lexeme });
            auto terminal_logical_or_op = GrammarSymbol(terminal_or_op_lexeme);
            auto terminal_logical_and_op = GrammarSymbol(terminal_logical_and_op_lexeme);
            auto terminal_inclusive_or_op = GrammarSymbol(terminal_inclusive_or_op_lexeme);
            auto terminal_exclusive_or_op = GrammarSymbol(terminal_exclusive_or_op_lexeme);
            auto terminal_bitwise_and_op = GrammarSymbol(terminal_bitwise_and_op_lexeme);
            auto terminal_equality_op1 = GrammarSymbol(terminal_equality_op1_lexeme);
            auto terminal_equality_op2 = GrammarSymbol(terminal_equality_op2_lexeme);
            auto terminal_add_op1 = GrammarSymbol(terminal_add_op1_lexeme);
            auto terminal_add_op2 = GrammarSymbol(terminal_add_op2_lexeme);
            auto terminal_mult_op1 = GrammarSymbol(terminal_mult_op1_lexeme);
            auto terminal_mult_op2 = GrammarSymbol(terminal_mult_op2_lexeme);
            auto terminal_mult_op3 = GrammarSymbol(terminal_mult_op3_lexeme);
            auto terminal_left_paren = GrammarSymbol(terminal_left_paren_lexeme);
            auto terminal_right_paren = GrammarSymbol(terminal_right_paren_lexeme);

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(assingment_expression);

            grammar
                (primary_expression, { { terminal }, { terminal_left_paren, expression, terminal_right_paren } })

                (multiplicative_expression, { { primary_expression, multiplicative_expression_ex } })
                (multiplicative_expression_ex, { { terminal_mult_op1, primary_expression, multiplicative_expression_ex }, { terminal_mult_op2, terminal, multiplicative_expression_ex }, { terminal_mult_op3, terminal, multiplicative_expression_ex }, { lambda_symbol } })

                (additive_expression, { { multiplicative_expression, additive_expression_ex } })
                (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, { terminal_add_op2, terminal, additive_expression_ex }, { lambda_symbol } })

                (equality_expression, { { additive_expression, equality_expression_ex } })
                (equality_expression_ex, { { terminal_equality_op1, additive_expression, equality_expression_ex }, { terminal_ass_op2, terminal, equality_expression_ex }, { lambda_symbol } })

                (and_expression, { { equality_expression, and_expression_ex } })
                (and_expression_ex, { { terminal_bitwise_and_op, equality_expression, and_expression_ex }, { lambda_symbol } })

                (exclusive_or_expression, { { and_expression, exclusive_or_expression_ex } })
                (exclusive_or_expression_ex, { { terminal_exclusive_or_op, and_expression, exclusive_or_expression_ex }, { lambda_symbol } })

                (inclusive_or_expression, { { exclusive_or_expression, inclusive_or_expression_ex } })
                (inclusive_or_expression_ex, { { terminal_inclusive_or_op, exclusive_or_expression, inclusive_or_expression_ex }, { lambda_symbol } })

                (logical_and_expression, { { inclusive_or_expression, logical_and_expression_ex } })
                (logical_and_expression_ex, { { terminal_logical_and_op, inclusive_or_expression, logical_and_expression_ex }, { lambda_symbol } })
                (logical_or_expression, { { logical_and_expression, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_logical_or_op, logical_and_expression, logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, { logical_or_expression } })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                (expression, { { assingment_expression } })
                ;

            TParsedString str2 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme };
            TParsedString str3 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme };
            TParsedString str4 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_or_op_lexeme, terminal_lexeme };
            TParsedString str5 = { terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_logical_and_op_lexeme, terminal_lexeme };
            TParsedString str6 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_exclusive_or_op_lexeme, terminal_lexeme };
            TParsedString str7 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme };
            TParsedString str8 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme };
            TParsedString str9 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op2_lexeme, terminal_lexeme };
            TParsedString str10 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op2_lexeme, terminal_lexeme };
            TParsedString str11 = { terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme };
            TParsedString str12 = { terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme };
            TParsedString str13 = {terminal_lexeme, terminal_ass_op1_lexeme, terminal_left_paren_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_mult_op1_lexeme, terminal_lexeme };

            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));
            Assert::IsTrue(sa.Analyze(str5));
            Assert::IsTrue(sa.Analyze(str6));
            Assert::IsTrue(sa.Analyze(str7));
            Assert::IsTrue(sa.Analyze(str8));
            Assert::IsTrue(sa.Analyze(str9));
            Assert::IsTrue(sa.Analyze(str10));
            Assert::IsTrue(sa.Analyze(str11));
            Assert::IsTrue(sa.Analyze(str12));
            Assert::IsTrue(sa.Analyze(str13));


            TParsedString inv_str_1 = { terminal_logical_and_op_lexeme };
            TParsedString inv_str_2 = { terminal_lexeme, terminal_add_op1_lexeme };
            TParsedString inv_str_3 = { terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme };
            TParsedString inv_str_4 = { terminal_lexeme, terminal_left_paren_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

        TEST_METHOD(SACppTest05)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto assingment_operator = GrammarSymbol("assignment-operator");
            auto assingment_expression = GrammarSymbol("assignment-expression");
            auto logical_or_expression = GrammarSymbol("logical-or-expression");
            auto logical_or_expression_ex = GrammarSymbol("logical-or-expression-ex");
            auto logical_and_expression = GrammarSymbol("logical-and-expression");
            auto logical_and_expression_ex = GrammarSymbol("logical-and-expression-ex");
            auto inclusive_or_expression = GrammarSymbol("inclusive-or-expression");
            auto inclusive_or_expression_ex = GrammarSymbol("inclusive-or-expression-ex");
            auto exclusive_or_expression = GrammarSymbol("exlusive-or-expression");
            auto exclusive_or_expression_ex = GrammarSymbol("exclusive-or-expression-ex");
            auto and_expression = GrammarSymbol("and-expression");
            auto and_expression_ex = GrammarSymbol("and-expression-ex");
            auto equality_expression = GrammarSymbol("equality-expression");
            auto equality_expression_ex = GrammarSymbol("equality-expression-ex");
            auto additive_expression = GrammarSymbol("additive-expression");
            auto additive_expression_ex = GrammarSymbol("additive-expression-ex");
            auto multiplicative_expression = GrammarSymbol("multiplicative-expression");
            auto multiplicative_expression_ex = GrammarSymbol("multiplicative-expression-ex");
            auto primary_expression = GrammarSymbol("primary-expression");
            auto expression = GrammarSymbol("expression");
            auto expression_optional = GrammarSymbol("expression-opt");
            auto expression_statement = GrammarSymbol("expression-statement");
            auto statement = GrammarSymbol("statement");
            auto statement_sequence = GrammarSymbol("statement-sequence");
            auto statement_sequence_optional = GrammarSymbol("statement-sequence-opt");
            auto compound_statement = GrammarSymbol("compound-statement");
            

            auto terminal_lexeme = LexemeInfo{ 0, "ID", 0 };
            auto terminal_ass_op1_lexeme = LexemeInfo{ 0, "=", 0 };
            auto terminal_ass_op2_lexeme = LexemeInfo{ 0, "+=", 0 };
            auto terminal_or_op_lexeme = LexemeInfo{ 0, "||", 0 };
            auto terminal_logical_and_op_lexeme = LexemeInfo{ 0, "&&", 0 };
            auto terminal_inclusive_or_op_lexeme = LexemeInfo{ 0, "|", 0 };
            auto terminal_exclusive_or_op_lexeme = LexemeInfo{ 0, "^", 0 };
            auto terminal_bitwise_and_op_lexeme = LexemeInfo{ 0, "&", 0 };
            auto terminal_equality_op1_lexeme = LexemeInfo{ 0, "==", 0 };
            auto terminal_equality_op2_lexeme = LexemeInfo{ 0, "!=", 0 };
            auto terminal_add_op1_lexeme = LexemeInfo{ 0, "+", 0 };
            auto terminal_add_op2_lexeme = LexemeInfo{ 0, "-", 0 };
            auto terminal_mult_op1_lexeme = LexemeInfo{ 0, "*", 0 };
            auto terminal_mult_op2_lexeme = LexemeInfo{ 0, "/", 0 };
            auto terminal_mult_op3_lexeme = LexemeInfo{ 0, "%", 0 };
            auto terminal_left_paren_lexeme = LexemeInfo{ 0, "(", 0 };
            auto terminal_right_paren_lexeme = LexemeInfo{ 0, ")", 0 };
            auto terminal_left_brace_lexeme = LexemeInfo{ 0, "{", 0 };
            auto terminal_right_brace_lexeme = LexemeInfo{ 0, "}", 0 };
            auto terminal_semicolon_lexeme = LexemeInfo{ 0, ";", 0 };

            auto terminal = GrammarSymbol(terminal_lexeme);
            auto terminal_ass_op1 = GrammarSymbol(terminal_ass_op1_lexeme);
            auto terminal_ass_op2 = GrammarSymbol(LexemeInfo{ terminal_ass_op2_lexeme });
            auto terminal_logical_or_op = GrammarSymbol(terminal_or_op_lexeme);
            auto terminal_logical_and_op = GrammarSymbol(terminal_logical_and_op_lexeme);
            auto terminal_inclusive_or_op = GrammarSymbol(terminal_inclusive_or_op_lexeme);
            auto terminal_exclusive_or_op = GrammarSymbol(terminal_exclusive_or_op_lexeme);
            auto terminal_bitwise_and_op = GrammarSymbol(terminal_bitwise_and_op_lexeme);
            auto terminal_equality_op1 = GrammarSymbol(terminal_equality_op1_lexeme);
            auto terminal_equality_op2 = GrammarSymbol(terminal_equality_op2_lexeme);
            auto terminal_add_op1 = GrammarSymbol(terminal_add_op1_lexeme);
            auto terminal_add_op2 = GrammarSymbol(terminal_add_op2_lexeme);
            auto terminal_mult_op1 = GrammarSymbol(terminal_mult_op1_lexeme);
            auto terminal_mult_op2 = GrammarSymbol(terminal_mult_op2_lexeme);
            auto terminal_mult_op3 = GrammarSymbol(terminal_mult_op3_lexeme);
            auto terminal_left_paren = GrammarSymbol(terminal_left_paren_lexeme);
            auto terminal_right_paren = GrammarSymbol(terminal_right_paren_lexeme);
            auto terminal_left_brace = GrammarSymbol(terminal_left_brace_lexeme);
            auto terminal_right_brace = GrammarSymbol(terminal_right_brace_lexeme);
            auto terminal_semicolon = GrammarSymbol(terminal_semicolon_lexeme);

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(compound_statement);

            grammar
                (compound_statement, { {terminal_left_brace, statement_sequence_optional, terminal_right_brace} })
                
                (statement_sequence_optional, { {statement_sequence}, {lambda_symbol} })
                (statement_sequence, { { statement }, { statement, statement_sequence } })
                (statement, { {expression_statement} })
                (expression_statement, { { expression_optional, terminal_semicolon } })
                (expression_optional, { { expression }, { lambda_symbol } })

                (primary_expression, { { terminal }, { terminal_left_paren, expression, terminal_right_paren } })

                (multiplicative_expression, { { primary_expression, multiplicative_expression_ex } })
                (multiplicative_expression_ex, { { terminal_mult_op1, primary_expression, multiplicative_expression_ex }, { terminal_mult_op2, terminal, multiplicative_expression_ex }, { terminal_mult_op3, terminal, multiplicative_expression_ex }, { lambda_symbol } })

                (additive_expression, { { multiplicative_expression, additive_expression_ex } })
                (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, { terminal_add_op2, terminal, additive_expression_ex }, { lambda_symbol } })

                (equality_expression, { { additive_expression, equality_expression_ex } })
                (equality_expression_ex, { { terminal_equality_op1, additive_expression, equality_expression_ex }, { terminal_ass_op2, terminal, equality_expression_ex }, { lambda_symbol } })

                (and_expression, { { equality_expression, and_expression_ex } })
                (and_expression_ex, { { terminal_bitwise_and_op, equality_expression, and_expression_ex }, { lambda_symbol } })

                (exclusive_or_expression, { { and_expression, exclusive_or_expression_ex } })
                (exclusive_or_expression_ex, { { terminal_exclusive_or_op, and_expression, exclusive_or_expression_ex }, { lambda_symbol } })

                (inclusive_or_expression, { { exclusive_or_expression, inclusive_or_expression_ex } })
                (inclusive_or_expression_ex, { { terminal_inclusive_or_op, exclusive_or_expression, inclusive_or_expression_ex }, { lambda_symbol } })

                (logical_and_expression, { { inclusive_or_expression, logical_and_expression_ex } })
                (logical_and_expression_ex, { { terminal_logical_and_op, inclusive_or_expression, logical_and_expression_ex }, { lambda_symbol } })
                (logical_or_expression, { { logical_and_expression, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_logical_or_op, logical_and_expression, logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, { logical_or_expression } })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                (expression, { { assingment_expression } })
                ;

            TParsedString str2 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str3 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str4 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_or_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str5 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_logical_and_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str6 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_exclusive_or_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str7 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str8 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str9 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op2_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str10 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op2_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str11 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str12 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str13 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_left_paren_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };

            TParsedString str14 = { terminal_left_brace_lexeme, terminal_right_brace_lexeme };
            TParsedString str15 = { terminal_left_brace_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            
            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));
            Assert::IsTrue(sa.Analyze(str5));
            Assert::IsTrue(sa.Analyze(str6));
            Assert::IsTrue(sa.Analyze(str7));
            Assert::IsTrue(sa.Analyze(str8));
            Assert::IsTrue(sa.Analyze(str9));
            Assert::IsTrue(sa.Analyze(str10));
            Assert::IsTrue(sa.Analyze(str11));
            Assert::IsTrue(sa.Analyze(str12));
            Assert::IsTrue(sa.Analyze(str13));
            Assert::IsTrue(sa.Analyze(str14));
            Assert::IsTrue(sa.Analyze(str15));

            TParsedString inv_str_1 = { terminal_left_brace_lexeme, terminal_logical_and_op_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_2 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_3 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_4 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_left_paren_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_5 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

        TEST_METHOD(SACppTest06)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;

            auto assingment_operator = GrammarSymbol("assignment-operator");
            auto assingment_expression = GrammarSymbol("assignment-expression");
            auto logical_or_expression = GrammarSymbol("logical-or-expression");
            auto logical_or_expression_ex = GrammarSymbol("logical-or-expression-ex");
            auto logical_and_expression = GrammarSymbol("logical-and-expression");
            auto logical_and_expression_ex = GrammarSymbol("logical-and-expression-ex");
            auto inclusive_or_expression = GrammarSymbol("inclusive-or-expression");
            auto inclusive_or_expression_ex = GrammarSymbol("inclusive-or-expression-ex");
            auto exclusive_or_expression = GrammarSymbol("exlusive-or-expression");
            auto exclusive_or_expression_ex = GrammarSymbol("exclusive-or-expression-ex");
            auto and_expression = GrammarSymbol("and-expression");
            auto and_expression_ex = GrammarSymbol("and-expression-ex");
            auto equality_expression = GrammarSymbol("equality-expression");
            auto equality_expression_ex = GrammarSymbol("equality-expression-ex");
            auto additive_expression = GrammarSymbol("additive-expression");
            auto additive_expression_ex = GrammarSymbol("additive-expression-ex");
            auto multiplicative_expression = GrammarSymbol("multiplicative-expression");
            auto multiplicative_expression_ex = GrammarSymbol("multiplicative-expression-ex");
            auto primary_expression = GrammarSymbol("primary-expression");
            auto expression = GrammarSymbol("expression");
            auto expression_optional = GrammarSymbol("expression-opt");
            auto expression_statement = GrammarSymbol("expression-statement");
            auto if_statement = GrammarSymbol("if-statement");
            auto while_statement = GrammarSymbol("while-statement");
            auto statement = GrammarSymbol("statement");
            auto statement_sequence = GrammarSymbol("statement-sequence");
            auto statement_sequence_optional = GrammarSymbol("statement-sequence-opt");
            auto compound_statement = GrammarSymbol("compound-statement");


            auto terminal_lexeme = LexemeInfo{ 0, "ID", 0 };
            auto terminal_ass_op1_lexeme = LexemeInfo{ 0, "=", 0 };
            auto terminal_ass_op2_lexeme = LexemeInfo{ 0, "+=", 0 };
            auto terminal_or_op_lexeme = LexemeInfo{ 0, "||", 0 };
            auto terminal_logical_and_op_lexeme = LexemeInfo{ 0, "&&", 0 };
            auto terminal_inclusive_or_op_lexeme = LexemeInfo{ 0, "|", 0 };
            auto terminal_exclusive_or_op_lexeme = LexemeInfo{ 0, "^", 0 };
            auto terminal_bitwise_and_op_lexeme = LexemeInfo{ 0, "&", 0 };
            auto terminal_equality_op1_lexeme = LexemeInfo{ 0, "==", 0 };
            auto terminal_equality_op2_lexeme = LexemeInfo{ 0, "!=", 0 };
            auto terminal_add_op1_lexeme = LexemeInfo{ 0, "+", 0 };
            auto terminal_add_op2_lexeme = LexemeInfo{ 0, "-", 0 };
            auto terminal_mult_op1_lexeme = LexemeInfo{ 0, "*", 0 };
            auto terminal_mult_op2_lexeme = LexemeInfo{ 0, "/", 0 };
            auto terminal_mult_op3_lexeme = LexemeInfo{ 0, "%", 0 };
            auto terminal_left_paren_lexeme = LexemeInfo{ 0, "(", 0 };
            auto terminal_right_paren_lexeme = LexemeInfo{ 0, ")", 0 };
            auto terminal_left_brace_lexeme = LexemeInfo{ 0, "{", 0 };
            auto terminal_right_brace_lexeme = LexemeInfo{ 0, "}", 0 };
            auto terminal_semicolon_lexeme = LexemeInfo{ 0, ";", 0 };
            auto terminal_if_lexeme = LexemeInfo{ 0, "if", 0 };
            auto terminal_else_lexeme = LexemeInfo{ 0, "else", 0 };
            auto terminal_while_lexeme = LexemeInfo{ 0, "while", 0 };

            auto terminal = GrammarSymbol(terminal_lexeme);
            auto terminal_ass_op1 = GrammarSymbol(terminal_ass_op1_lexeme);
            auto terminal_ass_op2 = GrammarSymbol(LexemeInfo{ terminal_ass_op2_lexeme });
            auto terminal_logical_or_op = GrammarSymbol(terminal_or_op_lexeme);
            auto terminal_logical_and_op = GrammarSymbol(terminal_logical_and_op_lexeme);
            auto terminal_inclusive_or_op = GrammarSymbol(terminal_inclusive_or_op_lexeme);
            auto terminal_exclusive_or_op = GrammarSymbol(terminal_exclusive_or_op_lexeme);
            auto terminal_bitwise_and_op = GrammarSymbol(terminal_bitwise_and_op_lexeme);
            auto terminal_equality_op1 = GrammarSymbol(terminal_equality_op1_lexeme);
            auto terminal_equality_op2 = GrammarSymbol(terminal_equality_op2_lexeme);
            auto terminal_add_op1 = GrammarSymbol(terminal_add_op1_lexeme);
            auto terminal_add_op2 = GrammarSymbol(terminal_add_op2_lexeme);
            auto terminal_mult_op1 = GrammarSymbol(terminal_mult_op1_lexeme);
            auto terminal_mult_op2 = GrammarSymbol(terminal_mult_op2_lexeme);
            auto terminal_mult_op3 = GrammarSymbol(terminal_mult_op3_lexeme);
            auto terminal_left_paren = GrammarSymbol(terminal_left_paren_lexeme);
            auto terminal_right_paren = GrammarSymbol(terminal_right_paren_lexeme);
            auto terminal_left_brace = GrammarSymbol(terminal_left_brace_lexeme);
            auto terminal_right_brace = GrammarSymbol(terminal_right_brace_lexeme);
            auto terminal_semicolon = GrammarSymbol(terminal_semicolon_lexeme);
            auto terminal_if = GrammarSymbol(terminal_if_lexeme);
            auto terminal_else = GrammarSymbol(terminal_else_lexeme);
            auto terminal_while = GrammarSymbol(terminal_while_lexeme);

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(statement);

            grammar
                (statement, { { expression_statement }, { if_statement }, { while_statement }, { compound_statement } })

                (compound_statement, { { terminal_left_brace, statement_sequence_optional, terminal_right_brace } })

                (statement_sequence_optional, { { statement_sequence }, { lambda_symbol } })
                (statement_sequence, { { statement }, { statement, statement_sequence } })

                (expression_statement, { { expression_optional, terminal_semicolon } })
                (expression_optional, { { expression }, { lambda_symbol } })

                (if_statement, { { terminal_if, terminal_left_paren, expression, terminal_right_paren, statement, terminal_else, statement }, { terminal_if, terminal_left_paren, expression, terminal_right_paren, statement } })

                (while_statement, { { terminal_while, terminal_left_paren, expression, terminal_right_paren, statement } })

                (primary_expression, { { terminal }, { terminal_left_paren, expression, terminal_right_paren } })

                (multiplicative_expression, { { primary_expression, multiplicative_expression_ex } })
                (multiplicative_expression_ex, { { terminal_mult_op1, primary_expression, multiplicative_expression_ex }, { terminal_mult_op2, terminal, multiplicative_expression_ex }, { terminal_mult_op3, terminal, multiplicative_expression_ex }, { lambda_symbol } })

                (additive_expression, { { multiplicative_expression, additive_expression_ex } })
                (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, { terminal_add_op2, terminal, additive_expression_ex }, { lambda_symbol } })

                (equality_expression, { { additive_expression, equality_expression_ex } })
                (equality_expression_ex, { { terminal_equality_op1, additive_expression, equality_expression_ex }, { terminal_ass_op2, terminal, equality_expression_ex }, { lambda_symbol } })

                (and_expression, { { equality_expression, and_expression_ex } })
                (and_expression_ex, { { terminal_bitwise_and_op, equality_expression, and_expression_ex }, { lambda_symbol } })

                (exclusive_or_expression, { { and_expression, exclusive_or_expression_ex } })
                (exclusive_or_expression_ex, { { terminal_exclusive_or_op, and_expression, exclusive_or_expression_ex }, { lambda_symbol } })

                (inclusive_or_expression, { { exclusive_or_expression, inclusive_or_expression_ex } })
                (inclusive_or_expression_ex, { { terminal_inclusive_or_op, exclusive_or_expression, inclusive_or_expression_ex }, { lambda_symbol } })

                (logical_and_expression, { { inclusive_or_expression, logical_and_expression_ex } })
                (logical_and_expression_ex, { { terminal_logical_and_op, inclusive_or_expression, logical_and_expression_ex }, { lambda_symbol } })
                (logical_or_expression, { { logical_and_expression, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_logical_or_op, logical_and_expression, logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, { logical_or_expression } })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                (expression, { { assingment_expression } })
                ;

            TParsedString str2 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str3 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str4 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_or_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str5 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op2_lexeme, terminal_lexeme, terminal_logical_and_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str6 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_exclusive_or_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str7 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme, terminal_inclusive_or_op_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str8 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str9 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op2_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str10 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op2_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str11 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str12 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str13 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_ass_op1_lexeme, terminal_left_paren_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_mult_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };

            TParsedString str14 = { terminal_left_brace_lexeme, terminal_right_brace_lexeme };
            TParsedString str15 = { terminal_left_brace_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };
            TParsedString str16 = { terminal_left_brace_lexeme, terminal_left_brace_lexeme, terminal_right_brace_lexeme, terminal_right_brace_lexeme };

            // if statement
            TParsedString str17 = { terminal_if_lexeme, terminal_left_paren_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_lexeme, terminal_semicolon_lexeme };
            TParsedString str18 = { terminal_if_lexeme, terminal_left_paren_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_else_lexeme, terminal_lexeme, terminal_semicolon_lexeme};

            // while statement
            TParsedString str19 = { terminal_while_lexeme, terminal_left_paren_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_left_brace_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme };

            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));
            Assert::IsTrue(sa.Analyze(str5));
            Assert::IsTrue(sa.Analyze(str6));
            Assert::IsTrue(sa.Analyze(str7));
            Assert::IsTrue(sa.Analyze(str8));
            Assert::IsTrue(sa.Analyze(str9));
            Assert::IsTrue(sa.Analyze(str10));
            Assert::IsTrue(sa.Analyze(str11));
            Assert::IsTrue(sa.Analyze(str12));
            Assert::IsTrue(sa.Analyze(str13));
            Assert::IsTrue(sa.Analyze(str14));
            Assert::IsTrue(sa.Analyze(str15));
            Assert::IsTrue(sa.Analyze(str16));
            Assert::IsTrue(sa.Analyze(str17));
            Assert::IsTrue(sa.Analyze(str18));
            Assert::IsTrue(sa.Analyze(str19));

            TParsedString inv_str_1 = { terminal_left_brace_lexeme, terminal_logical_and_op_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_2 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_3 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_4 = { terminal_left_brace_lexeme, terminal_lexeme, terminal_left_paren_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_right_paren_lexeme, terminal_right_brace_lexeme };
            TParsedString inv_str_5 = { terminal_lexeme, terminal_ass_op1_lexeme, terminal_lexeme };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

	};
}