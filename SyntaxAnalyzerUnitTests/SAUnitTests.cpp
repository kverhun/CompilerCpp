#include "stdafx.h"
#include "CppUnitTest.h"

#include <SyntaxAnalyzer\SyntaxAnalyzer.h>
#include <SyntaxAnalyzer\Grammar.h>
#include <SyntaxAnalyzer\GrammarGenerators.h>

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
            auto start_symbol = NonTerminal("S");
            Grammar g(start_symbol);

            auto terminal_1 = Terminal("A");
            auto terminal_2 = Terminal("B");
            auto terminal_3 = Terminal("C");

            g(start_symbol, { { terminal_1 }, { terminal_2 }, { terminal_3} });

            TSyntaxAnalyzerInput str1 = { Terminal("A"), };
            TSyntaxAnalyzerInput str2 = { Terminal("B"), };
            TSyntaxAnalyzerInput str3 = { Terminal("C"), };
            TSyntaxAnalyzerInput str4 = { Terminal("D"), };
            TSyntaxAnalyzerInput str5 = { Terminal("AB"), };
            TSyntaxAnalyzerInput str6 = { Terminal("A") ,Terminal("B") };

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
            auto start_symbol = NonTerminal("E");
            Grammar g(start_symbol);

            auto terminal_1 = Terminal("INT");
            auto terminal_2 = Terminal("+");

            g(start_symbol, { { terminal_1, terminal_2, start_symbol }, { terminal_1 } });

            TSyntaxAnalyzerInput str1 = { terminal_1 };
            TSyntaxAnalyzerInput str2 = { terminal_1, terminal_2, terminal_1};
            TSyntaxAnalyzerInput str3 = { Terminal("INT"), Terminal("+"), Terminal("INT"), Terminal("+"), Terminal("INT")};
            TSyntaxAnalyzerInput str4 = { terminal_1, terminal_1, terminal_1 };
            TSyntaxAnalyzerInput str5 = { terminal_1, terminal_1, terminal_2 };
            TSyntaxAnalyzerInput str6 = { terminal_2, terminal_1, terminal_1 };
            TSyntaxAnalyzerInput str7 = { terminal_2, terminal_2, terminal_1 };
            TSyntaxAnalyzerInput str8 = { terminal_1, terminal_2, terminal_2 };
            TSyntaxAnalyzerInput str9 = { terminal_2, terminal_2, terminal_2 };
            TSyntaxAnalyzerInput str10 = { terminal_2 };

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
            auto start_symbol = NonTerminal("E");
            Grammar g(start_symbol);

            auto terminal_1 = Terminal("INT");
            auto terminal_2 = Terminal("+");

            g(start_symbol, { { terminal_1 }, { terminal_1, terminal_2, start_symbol } });

            TSyntaxAnalyzerInput empty_str = { };

            SyntaxAnalyzer sa(g);

            Assert::IsFalse(sa.Analyze(empty_str));

        }

        //////////////////////////////////////////////////////////////////////////
        // Test lambda-productions
        //////////////////////////////////////////////////////////////////////////
        TEST_METHOD(SATest03)
        {
            auto start_symbol = NonTerminal("E");
            Grammar g(start_symbol);

            g(start_symbol, { { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });

            TSyntaxAnalyzerInput str1 = {};
            TSyntaxAnalyzerInput str2 = { Terminal("E") };

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsFalse(sa.Analyze(str2));

        }

        TEST_METHOD(SAShouldWorkCorrectWithLambdaProductions)
        {
            auto start_symbol = NonTerminal("E");
            Grammar g(start_symbol);

            auto terminal_1 = Terminal("INT");
            auto terminal_2 = Terminal("+");

            g(start_symbol, { { terminal_1, terminal_2, start_symbol }, { terminal_1 }, { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });

            TSyntaxAnalyzerInput empty_str = {};

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(empty_str));

            TSyntaxAnalyzerInput str1 = { terminal_1 };
            TSyntaxAnalyzerInput str2 = { terminal_1, terminal_2, terminal_1};
            TSyntaxAnalyzerInput str3 = { terminal_1, terminal_2, terminal_1, terminal_2, terminal_1 };

            TSyntaxAnalyzerInput str4 = { terminal_1, terminal_1, terminal_1 };
            TSyntaxAnalyzerInput str5 = { terminal_1, terminal_1, terminal_2 };
            TSyntaxAnalyzerInput str6 = { terminal_2, terminal_1, terminal_1 };
            TSyntaxAnalyzerInput str7 = { terminal_2, terminal_2, terminal_1 };
            TSyntaxAnalyzerInput str8 = { terminal_1, terminal_2, terminal_2 };
            TSyntaxAnalyzerInput str9 = { terminal_2, terminal_2, terminal_2 };
            TSyntaxAnalyzerInput str10 = { terminal_2 };

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
            auto start_symbol = NonTerminal("int-seq-opt");
            Grammar g(start_symbol);

            auto int_seq = NonTerminal("int-seq");
            auto terminal_1 = Terminal("INT");

            g(start_symbol, { { int_seq }, { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });
            g(int_seq, { { terminal_1, int_seq }, { terminal_1 } });

            TSyntaxAnalyzerInput str1 = {};
            TSyntaxAnalyzerInput str2 = { terminal_1 };
            TSyntaxAnalyzerInput str3 = { terminal_1, terminal_1};
            TSyntaxAnalyzerInput str4 = { terminal_1, terminal_1, terminal_1 };

            SyntaxAnalyzer sa(g);

            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));

        }

        TEST_METHOD(SACppTest01)
        {
            auto assingment_operator =   NonTerminal("assignment-operator");
            auto assingment_expression = NonTerminal("assignment-expression");
            auto logical_or_expression = NonTerminal("logical-or-expression");

            auto terminal =         Terminal("ID");
            auto terminal_or_op =   Terminal("||");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");

            auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

            Grammar grammar(assingment_expression);
            auto logical_or_expression_ex = NonTerminal("logical-or-expression-ex");

            grammar
                (logical_or_expression, { { terminal, logical_or_expression_ex } })
                (logical_or_expression_ex, { { terminal_or_op, terminal, logical_or_expression_ex }, { lambda_symbol } })
                (assingment_expression, { { logical_or_expression, assingment_operator, assingment_expression }, {logical_or_expression} })
                (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
                ;

            TSyntaxAnalyzerInput str2 = { terminal, terminal_ass_op1, terminal };
            TSyntaxAnalyzerInput str3 = { terminal, terminal_ass_op2, terminal };
            TSyntaxAnalyzerInput str4 = { terminal, terminal_ass_op2, terminal, terminal_or_op, terminal};

            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str2));
            Assert::IsTrue(sa.Analyze(str3));
            Assert::IsTrue(sa.Analyze(str4));
        }

        TEST_METHOD(SACppTest02)
        {
            auto assingment_operator = NonTerminal("assignment-operator");
            auto assingment_expression = NonTerminal("assignment-expression");
            auto logical_or_expression = NonTerminal("logical-or-expression");
            auto logical_or_expression_ex = NonTerminal("logical-or-expression-ex");
            auto logical_and_expression = NonTerminal("logical-and-expression");
            auto logical_and_expression_ex = NonTerminal("logical-and-expression-ex");
            auto inclusive_or_expression = NonTerminal("inclusive-or-expression");
            auto inclusive_or_expression_ex = NonTerminal("inclusive-or-expression-ex");
            auto exclusive_or_expression = NonTerminal("exlusive-or-expression");
            auto exclusive_or_expression_ex = NonTerminal("exclusive-or-expression-ex");
            auto and_expression = NonTerminal("and-expression");
            auto and_expression_ex = NonTerminal("and-expression-ex");
            auto equality_expression = NonTerminal("equality-expression");
            auto equality_expression_ex = NonTerminal("equality-expression-ex");
            auto additive_expression = NonTerminal("additive-expression");
            auto additive_expression_ex = NonTerminal("additive-expression-ex");
            auto multiplicative_expression = NonTerminal("multiplicative-expression");
            auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");

            auto terminal_ass_op1 =         Terminal("=");
            auto terminal_ass_op2 =         Terminal("+=");
            auto terminal =                 Terminal("ID");
            auto terminal_logical_or_op =   Terminal("||");
            auto terminal_logical_and_op =  Terminal("&&");
            auto terminal_inclusive_or_op = Terminal("|");
            auto terminal_exclusive_or_op = Terminal("^");
            auto terminal_bitwise_and_op =  Terminal("&");
            auto terminal_equality_op1 =    Terminal("==");
            auto terminal_equality_op2 =    Terminal("!=");
            auto terminal_add_op1 =         Terminal("+");
            auto terminal_add_op2 =         Terminal("-");
            auto terminal_mult_op1 =        Terminal("*");
            auto terminal_mult_op2 =        Terminal("/");
            auto terminal_mult_op3 =        Terminal("&");

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

            TSyntaxAnalyzerInput str2 = { terminal, terminal_ass_op1, terminal };
            TSyntaxAnalyzerInput str3 = { terminal, terminal_ass_op2, terminal };
            TSyntaxAnalyzerInput str4 = { terminal, terminal_ass_op2, terminal, terminal_logical_or_op, terminal};
            TSyntaxAnalyzerInput str5 = { terminal, terminal_ass_op2, terminal, terminal_logical_and_op, terminal};
            TSyntaxAnalyzerInput str6 = { terminal, terminal_ass_op1, terminal, terminal_exclusive_or_op, terminal };
            TSyntaxAnalyzerInput str7 = { terminal, terminal_ass_op1, terminal, terminal_inclusive_or_op, terminal, terminal_inclusive_or_op, terminal};
            TSyntaxAnalyzerInput str8 = { terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal};
            TSyntaxAnalyzerInput str9 = { terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_add_op2, terminal};
            TSyntaxAnalyzerInput str10 = { terminal, terminal_ass_op1, terminal, terminal_add_op1, terminal, terminal_mult_op2, terminal};
            
            
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
            

            TSyntaxAnalyzerInput inv_str_1 = { terminal_logical_and_op};
            TSyntaxAnalyzerInput inv_str_2 = { terminal, terminal_add_op1};
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
        }

        TEST_METHOD(SACppTest03)
        {
            auto expression = NonTerminal("expression");

            auto additive_expression = NonTerminal("additive-expression");
            auto additive_expression_ex = NonTerminal("additive-expression-ex");
            auto multiplicative_expression = NonTerminal("multiplicative-expression");
            auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");
            auto primary_expression = NonTerminal("primary-expression");

            auto terminal =             Terminal("ID");
            auto terminal_add_op1 =     Terminal("+");
            auto terminal_add_op2 =     Terminal("-");
            auto terminal_mult_op1 =    Terminal("*");
            auto terminal_mult_op2 =    Terminal("/");
            auto terminal_mult_op3 =    Terminal("%");
            auto terminal_left_paren =  Terminal("(");
            auto terminal_right_paren = Terminal(")");

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

            TSyntaxAnalyzerInput str1 = { terminal, terminal_mult_op1, terminal, terminal_add_op1, terminal};
            TSyntaxAnalyzerInput str2 = { terminal, terminal_add_op1, terminal, terminal_mult_op1, terminal};

            SyntaxAnalyzer sa(grammar);
            Assert::IsTrue(sa.Analyze(str1));
            Assert::IsTrue(sa.Analyze(str2));
        }

        TEST_METHOD(SACppTest04)
        {
            auto assingment_operator = NonTerminal("assignment-operator");
            auto assingment_expression = NonTerminal("assignment-expression");
            auto logical_or_expression = NonTerminal("logical-or-expression");
            auto logical_or_expression_ex = NonTerminal("logical-or-expression-ex");
            auto logical_and_expression = NonTerminal("logical-and-expression");
            auto logical_and_expression_ex = NonTerminal("logical-and-expression-ex");
            auto inclusive_or_expression = NonTerminal("inclusive-or-expression");
            auto inclusive_or_expression_ex = NonTerminal("inclusive-or-expression-ex");
            auto exclusive_or_expression = NonTerminal("exlusive-or-expression");
            auto exclusive_or_expression_ex = NonTerminal("exclusive-or-expression-ex");
            auto and_expression = NonTerminal("and-expression");
            auto and_expression_ex = NonTerminal("and-expression-ex");
            auto equality_expression = NonTerminal("equality-expression");
            auto equality_expression_ex = NonTerminal("equality-expression-ex");
            auto additive_expression = NonTerminal("additive-expression");
            auto additive_expression_ex = NonTerminal("additive-expression-ex");
            auto multiplicative_expression = NonTerminal("multiplicative-expression");
            auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");
            auto primary_expression = NonTerminal("primary-expression");
            auto expression = NonTerminal("expression");

            auto terminal = Terminal("ID");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");
            auto terminal_logical_or_op = Terminal("||");
            auto terminal_logical_and_op = Terminal("&&");
            auto terminal_inclusive_or_op = Terminal("|");
            auto terminal_exclusive_or_op = Terminal("^");
            auto terminal_bitwise_and_op = Terminal("&");
            auto terminal_equality_op1 = Terminal("==");
            auto terminal_equality_op2 = Terminal("!=");
            auto terminal_add_op1 = Terminal("+");
            auto terminal_add_op2 = Terminal("-");
            auto terminal_mult_op1 = Terminal("*");
            auto terminal_mult_op2 = Terminal("/");
            auto terminal_mult_op3 = Terminal("%");
            auto terminal_left_paren = Terminal("(");
            auto terminal_right_paren = Terminal(")");

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

            TSyntaxAnalyzerInput str2 = { terminal, terminal_ass_op1, terminal };
            TSyntaxAnalyzerInput str3 = { terminal, terminal_ass_op2, terminal };
            TSyntaxAnalyzerInput str4 = { terminal, terminal_ass_op2, terminal, terminal_logical_or_op, terminal };
            TSyntaxAnalyzerInput str5 = { terminal, terminal_ass_op2, terminal, terminal_logical_and_op, terminal };
            TSyntaxAnalyzerInput str6 = { terminal, terminal_ass_op1, terminal, terminal_exclusive_or_op, terminal};
            TSyntaxAnalyzerInput str7 = { terminal, terminal_ass_op1, terminal, terminal_inclusive_or_op, terminal, terminal_inclusive_or_op, terminal};
            TSyntaxAnalyzerInput str8 = { terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal };
            TSyntaxAnalyzerInput str9 = { terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_add_op2, terminal };
            TSyntaxAnalyzerInput str10 = { terminal, terminal_ass_op1, terminal, terminal_add_op1, terminal, terminal_mult_op2, terminal };
            TSyntaxAnalyzerInput str11 = { terminal, terminal_mult_op1, terminal, terminal_add_op1, terminal };
            TSyntaxAnalyzerInput str12 = { terminal, terminal_add_op1, terminal, terminal_mult_op1, terminal };
            TSyntaxAnalyzerInput str13 = {terminal, terminal_ass_op1, terminal_left_paren, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_mult_op1, terminal };

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


            TSyntaxAnalyzerInput inv_str_1 = { terminal_logical_and_op };
            TSyntaxAnalyzerInput inv_str_2 = { terminal, terminal_add_op1 };
            TSyntaxAnalyzerInput inv_str_3 = { terminal, terminal_add_op1, terminal, terminal_right_paren };
            TSyntaxAnalyzerInput inv_str_4 = { terminal, terminal_left_paren, terminal_add_op1, terminal, terminal_right_paren };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

        TEST_METHOD(SACppTest05)
        {
            auto assingment_operator = NonTerminal("assignment-operator");
            auto assingment_expression = NonTerminal("assignment-expression");
            auto logical_or_expression = NonTerminal("logical-or-expression");
            auto logical_or_expression_ex = NonTerminal("logical-or-expression-ex");
            auto logical_and_expression = NonTerminal("logical-and-expression");
            auto logical_and_expression_ex = NonTerminal("logical-and-expression-ex");
            auto inclusive_or_expression = NonTerminal("inclusive-or-expression");
            auto inclusive_or_expression_ex = NonTerminal("inclusive-or-expression-ex");
            auto exclusive_or_expression = NonTerminal("exlusive-or-expression");
            auto exclusive_or_expression_ex = NonTerminal("exclusive-or-expression-ex");
            auto and_expression = NonTerminal("and-expression");
            auto and_expression_ex = NonTerminal("and-expression-ex");
            auto equality_expression = NonTerminal("equality-expression");
            auto equality_expression_ex = NonTerminal("equality-expression-ex");
            auto additive_expression = NonTerminal("additive-expression");
            auto additive_expression_ex = NonTerminal("additive-expression-ex");
            auto multiplicative_expression = NonTerminal("multiplicative-expression");
            auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");
            auto primary_expression = NonTerminal("primary-expression");
            auto expression = NonTerminal("expression");
            auto expression_optional = NonTerminal("expression-opt");
            auto expression_statement = NonTerminal("expression-statement");
            auto statement = NonTerminal("statement");
            auto statement_sequence = NonTerminal("statement-sequence");
            auto statement_sequence_optional = NonTerminal("statement-sequence-opt");
            auto compound_statement = NonTerminal("compound-statement");
            

            auto terminal_lexeme =                  LexemeInfo{ 0, "ID"  , 0 };
            auto terminal_ass_op1_lexeme =          LexemeInfo{ 0, "="  , 0 };
            auto terminal_ass_op2_lexeme =          LexemeInfo{ 0, "+=" , 0 };
            auto terminal_or_op_lexeme =            LexemeInfo{ 0, "||" , 0 };
            auto terminal_logical_and_op_lexeme =   LexemeInfo{ 0, "&&" , 0 };
            auto terminal_inclusive_or_op_lexeme =  LexemeInfo{ 0, "|"  , 0 };
            auto terminal_exclusive_or_op_lexeme =  LexemeInfo{ 0, "^"  , 0 };
            auto terminal_bitwise_and_op_lexeme =   LexemeInfo{ 0, "&"  , 0 };
            auto terminal_equality_op1_lexeme =     LexemeInfo{ 0, "==" , 0 };
            auto terminal_equality_op2_lexeme =     LexemeInfo{ 0, "!=" , 0 };
            auto terminal_add_op1_lexeme =          LexemeInfo{ 0, "+"  , 0 };
            auto terminal_add_op2_lexeme =          LexemeInfo{ 0, "-"  , 0 };
            auto terminal_mult_op1_lexeme =         LexemeInfo{ 0, "*"  , 0 };
            auto terminal_mult_op2_lexeme =         LexemeInfo{ 0, "/"  , 0 };
            auto terminal_mult_op3_lexeme =         LexemeInfo{ 0, "%"  , 0 };
            auto terminal_left_paren_lexeme =       LexemeInfo{ 0, "("  , 0 };
            auto terminal_right_paren_lexeme =      LexemeInfo{ 0, ")"  , 0 };
            auto terminal_left_brace_lexeme =       LexemeInfo{ 0, "{"  , 0 };
            auto terminal_right_brace_lexeme =      LexemeInfo{ 0, "}"  , 0 };
            auto terminal_semicolon_lexeme =        LexemeInfo{ 0, ";"  , 0 };

            auto terminal = Terminal("ID");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");
            auto terminal_logical_or_op = Terminal("||");
            auto terminal_logical_and_op = Terminal("&&");
            auto terminal_inclusive_or_op = Terminal("|");
            auto terminal_exclusive_or_op = Terminal("^");
            auto terminal_bitwise_and_op = Terminal("&");
            auto terminal_equality_op1 = Terminal("==");
            auto terminal_equality_op2 = Terminal("!=");
            auto terminal_add_op1 = Terminal("+");
            auto terminal_add_op2 = Terminal("-");
            auto terminal_mult_op1 = Terminal("*");
            auto terminal_mult_op2 = Terminal("/");
            auto terminal_mult_op3 = Terminal("%");
            auto terminal_left_paren = Terminal("(");
            auto terminal_right_paren = Terminal(")");
            auto terminal_left_brace = Terminal("{");
            auto terminal_right_brace = Terminal("}");
            auto terminal_semicolon = Terminal(";");

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

            TSyntaxAnalyzerInput str2 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str3 = { terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str4 = { terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_logical_or_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str5 = { terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_logical_and_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str6 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_exclusive_or_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str7 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_inclusive_or_op, terminal, terminal_inclusive_or_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str8 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str9 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_add_op2, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str10 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_add_op1, terminal, terminal_mult_op2, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str11 = { terminal_left_brace, terminal, terminal_mult_op1, terminal, terminal_add_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str12 = { terminal_left_brace, terminal, terminal_add_op1, terminal, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str13 = { terminal_left_brace, terminal, terminal_ass_op1, terminal_left_paren, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace };

            TSyntaxAnalyzerInput str14 = { terminal_left_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str15 = { terminal_left_brace, terminal_semicolon, terminal_right_brace };
            
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

            TSyntaxAnalyzerInput inv_str_1 = { terminal_left_brace, terminal_logical_and_op, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_2 = { terminal_left_brace, terminal, terminal_add_op1, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_3 = { terminal_left_brace, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_4 = { terminal_left_brace, terminal, terminal_left_paren, terminal_add_op1, terminal, terminal_right_paren, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_5 = { terminal, terminal_ass_op1, terminal };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

        TEST_METHOD(SACppTest06)
        {
            auto assingment_operator = NonTerminal("assignment-operator");
            auto assingment_expression = NonTerminal("assignment-expression");
            auto logical_or_expression = NonTerminal("logical-or-expression");
            auto logical_or_expression_ex = NonTerminal("logical-or-expression-ex");
            auto logical_and_expression = NonTerminal("logical-and-expression");
            auto logical_and_expression_ex = NonTerminal("logical-and-expression-ex");
            auto inclusive_or_expression = NonTerminal("inclusive-or-expression");
            auto inclusive_or_expression_ex = NonTerminal("inclusive-or-expression-ex");
            auto exclusive_or_expression = NonTerminal("exlusive-or-expression");
            auto exclusive_or_expression_ex = NonTerminal("exclusive-or-expression-ex");
            auto and_expression = NonTerminal("and-expression");
            auto and_expression_ex = NonTerminal("and-expression-ex");
            auto equality_expression = NonTerminal("equality-expression");
            auto equality_expression_ex = NonTerminal("equality-expression-ex");
            auto additive_expression = NonTerminal("additive-expression");
            auto additive_expression_ex = NonTerminal("additive-expression-ex");
            auto multiplicative_expression = NonTerminal("multiplicative-expression");
            auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");
            auto primary_expression = NonTerminal("primary-expression");
            auto expression = NonTerminal("expression");
            auto expression_optional = NonTerminal("expression-opt");
            auto expression_statement = NonTerminal("expression-statement");
            auto if_statement = NonTerminal("if-statement");
            auto while_statement = NonTerminal("while-statement");
            auto statement = NonTerminal("statement");
            auto statement_sequence = NonTerminal("statement-sequence");
            auto statement_sequence_optional = NonTerminal("statement-sequence-opt");
            auto compound_statement = NonTerminal("compound-statement");


            auto terminal_lexeme =                  LexemeInfo{ 0, "ID"     , 0 };
            auto terminal_ass_op1_lexeme =          LexemeInfo{ 0, "="      , 0 };
            auto terminal_ass_op2_lexeme =          LexemeInfo{ 0, "+="     , 0 };
            auto terminal_or_op_lexeme =            LexemeInfo{ 0, "||"    , 0 };
            auto terminal_logical_and_op_lexeme =   LexemeInfo{ 0, "&&"    , 0 };
            auto terminal_inclusive_or_op_lexeme =  LexemeInfo{ 0, "|"     ,  0 };
            auto terminal_exclusive_or_op_lexeme =  LexemeInfo{ 0, "^"     , 0 };
            auto terminal_bitwise_and_op_lexeme =   LexemeInfo{ 0, "&"     , 0 };
            auto terminal_equality_op1_lexeme =     LexemeInfo{ 0, "=="    , 0 };
            auto terminal_equality_op2_lexeme =     LexemeInfo{ 0, "!="    , 0 };
            auto terminal_add_op1_lexeme =          LexemeInfo{ 0, "+"     , 0 };
            auto terminal_add_op2_lexeme =          LexemeInfo{ 0, "-"     , 0 };
            auto terminal_mult_op1_lexeme =         LexemeInfo{ 0, "*"     , 0 };
            auto terminal_mult_op2_lexeme =         LexemeInfo{ 0, "/"     , 0 };
            auto terminal_mult_op3_lexeme =         LexemeInfo{ 0, "%"     , 0 };
            auto terminal_left_paren_lexeme =       LexemeInfo{ 0, "("     , 0 };
            auto terminal_right_paren_lexeme =      LexemeInfo{ 0, ")"     , 0 };
            auto terminal_left_brace_lexeme =       LexemeInfo{ 0, "{"     , 0 };
            auto terminal_right_brace_lexeme =      LexemeInfo{ 0, "}"     , 0 };
            auto terminal_semicolon_lexeme =        LexemeInfo{ 0, ";"     , 0 };
            auto terminal_if_lexeme =               LexemeInfo{ 0, "if"    , 0 };
            auto terminal_else_lexeme =             LexemeInfo{ 0, "else"  , 0 };
            auto terminal_while_lexeme =            LexemeInfo{ 0, "while", 0 };

            auto terminal = Terminal("ID");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");
            auto terminal_logical_or_op = Terminal("||");
            auto terminal_logical_and_op = Terminal("&&");
            auto terminal_inclusive_or_op = Terminal("|");
            auto terminal_exclusive_or_op = Terminal("^");
            auto terminal_bitwise_and_op = Terminal("&");
            auto terminal_equality_op1 = Terminal("==");
            auto terminal_equality_op2 = Terminal("!=");
            auto terminal_add_op1 = Terminal("+");
            auto terminal_add_op2 = Terminal("-");
            auto terminal_mult_op1 = Terminal("*");
            auto terminal_mult_op2 = Terminal("/");
            auto terminal_mult_op3 = Terminal("%");
            auto terminal_left_paren = Terminal("(");
            auto terminal_right_paren = Terminal(")");
            auto terminal_left_brace = Terminal("{");
            auto terminal_right_brace = Terminal("}");
            auto terminal_semicolon = Terminal(";");
            auto terminal_if = Terminal("if");
            auto terminal_else = Terminal("else");
            auto terminal_while = Terminal("while");

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

            TSyntaxAnalyzerInput str2 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str3 = { terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str4 = { terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_logical_or_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str5 = { terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_logical_and_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str6 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_exclusive_or_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str7 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_inclusive_or_op, terminal, terminal_inclusive_or_op, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str8 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str9 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_add_op2, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str10 = { terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_add_op1, terminal, terminal_mult_op2, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str11 = { terminal_left_brace, terminal, terminal_mult_op1, terminal, terminal_add_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str12 = { terminal_left_brace, terminal, terminal_add_op1, terminal, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str13 = { terminal_left_brace, terminal, terminal_ass_op1, terminal_left_paren, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace };

            TSyntaxAnalyzerInput str14 = { terminal_left_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str15 = { terminal_left_brace, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str16 = { terminal_left_brace, terminal_left_brace, terminal_right_brace, terminal_right_brace };

            // if statement
            TSyntaxAnalyzerInput str17 = { terminal_if, terminal_left_paren, terminal, terminal_right_paren, terminal, terminal_semicolon };
            TSyntaxAnalyzerInput str18 = { terminal_if, terminal_left_paren, terminal, terminal_right_paren, terminal, terminal_semicolon, terminal_else, terminal, terminal_semicolon};

            // while statement
            TSyntaxAnalyzerInput str19 = { terminal_while, terminal_left_paren, terminal, terminal_right_paren, terminal_left_brace, terminal, terminal_semicolon, terminal_right_brace };

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

            TSyntaxAnalyzerInput inv_str_1 = { terminal_left_brace, terminal_logical_and_op, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_2 = { terminal_left_brace, terminal, terminal_add_op1, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_3 = { terminal_left_brace, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_4 = { terminal_left_brace, terminal, terminal_left_paren, terminal_add_op1, terminal, terminal_right_paren, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_5 = { terminal, terminal_ass_op1, terminal };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

        TEST_METHOD(SAShouldGenerateCppGrammar)
        {
            auto p_grammar = GenerateGrammarCpp();

            auto terminal = Terminal("ID");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");
            auto terminal_logical_or_op = Terminal("||");
            auto terminal_logical_and_op = Terminal("&&");
            auto terminal_inclusive_or_op = Terminal("|");
            auto terminal_exclusive_or_op = Terminal("^");
            auto terminal_bitwise_and_op = Terminal("&");
            auto terminal_equality_op1 = Terminal("==");
            auto terminal_equality_op2 = Terminal("!=");
            auto terminal_add_op1 = Terminal("+");
            auto terminal_add_op2 = Terminal("-");
            auto terminal_mult_op1 = Terminal("*");
            auto terminal_mult_op2 = Terminal("/");
            auto terminal_mult_op3 = Terminal("%");
            auto terminal_left_paren = Terminal("(");
            auto terminal_right_paren = Terminal(")");
            auto terminal_left_brace = Terminal("{");
            auto terminal_right_brace = Terminal("}");
            auto terminal_semicolon = Terminal(";");
            auto terminal_if = Terminal("if");
            auto terminal_else = Terminal("else");
            auto terminal_while = Terminal("while");


            TSyntaxAnalyzerInput str2 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace};
            TSyntaxAnalyzerInput str3 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str4 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_logical_or_op, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str5 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op2, terminal, terminal_logical_and_op, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str6 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_exclusive_or_op, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str7 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_inclusive_or_op, terminal, terminal_inclusive_or_op, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str8 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str9 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_mult_op1, terminal, terminal_add_op2, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str10 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_add_op1, terminal, terminal_mult_op2, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str11 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_mult_op1, terminal, terminal_add_op1, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str12 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_add_op1, terminal, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str13 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_ass_op1, terminal_left_paren, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_mult_op1, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };

            TSyntaxAnalyzerInput str14 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str15 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str16 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal_left_brace, terminal_right_brace, terminal_right_brace, terminal_right_brace };

            
            // if statement
            TSyntaxAnalyzerInput str17 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_if, terminal_left_paren, terminal, terminal_right_paren, terminal, terminal_semicolon, terminal_right_brace };
            TSyntaxAnalyzerInput str18 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_if, terminal_left_paren, terminal, terminal_right_paren, terminal, terminal_semicolon, terminal_else, terminal, terminal_semicolon, terminal_right_brace };

            // while statement
            TSyntaxAnalyzerInput str19 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_while, terminal_left_paren, terminal, terminal_right_paren, terminal_left_brace, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str20 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_while, terminal_left_paren, terminal, terminal_right_paren, terminal_left_brace, terminal, terminal_semicolon, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };

            SyntaxAnalyzer sa(*p_grammar);
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

            TSyntaxAnalyzerInput inv_str_1 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal_logical_and_op, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_2 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_add_op1, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_3 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_add_op1, terminal, terminal_right_paren, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_4 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_left_paren, terminal_add_op1, terminal, terminal_right_paren, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput inv_str_5 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal, terminal_ass_op1, terminal, terminal_right_brace };
            Assert::IsFalse(sa.Analyze(inv_str_1));
            Assert::IsFalse(sa.Analyze(inv_str_2));
            Assert::IsFalse(sa.Analyze(inv_str_3));
            Assert::IsFalse(sa.Analyze(inv_str_4));
        }

        TEST_METHOD(ShouldWorkWithBlockCorrectly)
        {
            auto p_grammar = GenerateGrammarCpp();

            auto terminal = Terminal("ID");
            auto terminal_ass_op1 = Terminal("=");
            auto terminal_ass_op2 = Terminal("+=");
            auto terminal_logical_or_op = Terminal("||");
            auto terminal_logical_and_op = Terminal("&&");
            auto terminal_inclusive_or_op = Terminal("|");
            auto terminal_exclusive_or_op = Terminal("^");
            auto terminal_bitwise_and_op = Terminal("&");
            auto terminal_equality_op1 = Terminal("==");
            auto terminal_equality_op2 = Terminal("!=");
            auto terminal_add_op1 = Terminal("+");
            auto terminal_add_op2 = Terminal("-");
            auto terminal_mult_op1 = Terminal("*");
            auto terminal_mult_op2 = Terminal("/");
            auto terminal_mult_op3 = Terminal("%");
            auto terminal_left_paren = Terminal("(");
            auto terminal_right_paren = Terminal(")");
            auto terminal_left_brace = Terminal("{");
            auto terminal_right_brace = Terminal("}");
            auto terminal_semicolon = Terminal(";");
            auto terminal_if = Terminal("if");
            auto terminal_else = Terminal("else");
            auto terminal_while = Terminal("while");

            // block test
            TSyntaxAnalyzerInput str_block_1 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str_block_2 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_semicolon, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };
            TSyntaxAnalyzerInput str_block_3 = { terminal, terminal, terminal_left_paren, terminal_right_paren, terminal_left_brace, terminal_left_brace, terminal, terminal_semicolon, terminal, terminal_semicolon, terminal, terminal_semicolon, terminal_right_brace, terminal_right_brace };

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);

            Assert::IsTrue(sa.Analyze(str_block_1));
            Assert::IsTrue(sa.Analyze(str_block_2));
            Assert::IsTrue(sa.Analyze(str_block_3));
        }

        TEST_METHOD(ShouldReturnCorrectProductionSequence1)
        {
            auto start_symbol = NonTerminal("S");
            Grammar g(start_symbol);

            auto terminal_1 = Terminal("A");
            auto terminal_2 = Terminal("B");
            auto terminal_3 = Terminal("C");

            g(start_symbol, { { terminal_1 }, { terminal_2 }, { terminal_3 } });

            TSyntaxAnalyzerInput str1 = { terminal_1 };
            TSyntaxAnalyzerInput str2 = { terminal_2 };
            TSyntaxAnalyzerInput str3 = { terminal_3 };

            TSyntaxAnalyzerInput str4 = { Terminal("D")};
            TSyntaxAnalyzerInput str5 = { Terminal("AB") };
            TSyntaxAnalyzerInput str6 = { terminal_1, terminal_2};

            std::vector<size_t> v1, v2, v3;

            SyntaxAnalyzer sa(g);
            bool res1 = sa.Analyze(v1, str1);
            bool res2 = sa.Analyze(v2, str2);
            bool res3 = sa.Analyze(v3, str3);

            Assert::IsTrue(res1);
            Assert::IsTrue(res2);
            Assert::IsTrue(res3);

            Assert::IsTrue(1 == v1.size());
            Assert::IsTrue(1 == v1[0]);
            Assert::IsTrue(1 == v2.size());
            Assert::IsTrue(2 == v2[0]);
            Assert::IsTrue(1 == v3.size());
            Assert::IsTrue(3 == v3[0]);

            Assert::IsFalse(sa.Analyze(str4));
            Assert::IsFalse(sa.Analyze(str5));
            Assert::IsFalse(sa.Analyze(str6));
        }


	};
}