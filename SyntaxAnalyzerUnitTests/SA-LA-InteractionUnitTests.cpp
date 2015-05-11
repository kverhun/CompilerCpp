#include "stdafx.h"
#include "CppUnitTest.h"

#include <LexicalAnalyzer\LexicalAnalyzer.h>
#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include <LexicalAnalyzer\LanguageInfoCpp.h>
#include <SyntaxAnalyzer\SyntaxAnalyzer.h>
#include <SyntaxAnalyzer\Grammar.h>
#include <SyntaxAnalyzer\GrammarGenerators.h>
#include <SyntaxAnalyzer\GrammarSymbol.h>
#include <SyntaxAnalyzer\SyntaxAnalysisHelpers.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <Interfaces\SyntaxAnalyzer\SyntaxAnalyzerInput.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SyntaxAnalyzerUnitTests
{
	TEST_CLASS(SALAInteractionUnitTests)
	{
	public:
        TEST_METHOD(ComplexTest1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string = "if (ID) {ID = ID + ID;} else ID = ID + ID;";
            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            Assert::IsTrue(sa.Analyze(SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res)));

        }

        TEST_METHOD(SALATest2)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string = 
                "while(ID) \
                 {      \
                    ID = ID + (ID * ID); \
                    if (ID = ID+ID) \
                    { ID; } \
                    else   \
                    {      \
                        ID = ID + ID * ID * (ID + ID); \
                        ID = ID + ID;  \
                    }                  \
                 }";
            
                    
            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            Assert::IsTrue(sa.Analyze(SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res)));

        }

        TEST_METHOD(SALAShouldReturnCorrectProductionSeq1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;
            using SyntaxAnalysis::GrammarSymbol;
            using SyntaxAnalysis::Terminal;
            using SyntaxAnalysis::NonTerminal;
            using SyntaxAnalysis::Grammar;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string = "ID;";

            auto res = cpp_analyzer.ParseString(string);
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

            std::unique_ptr<Grammar> p_grammar(new Grammar(statement));

            (*p_grammar)
                (statement, { { expression_statement }, { if_statement }, { while_statement }, { compound_statement } })

                (compound_statement, { { terminal_left_brace, statement_sequence_optional, terminal_right_brace } })

                (statement_sequence_optional, { { statement_sequence }, { lambda_symbol } })
                (statement_sequence, { { statement, statement_sequence }, { statement } })

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

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            Assert::IsTrue(sa_res);

            Assert::IsTrue(22 == v.size());

            Assert::IsTrue(1 == v[0]);
            Assert::IsTrue(1 == v[1]);
            Assert::IsTrue(1 == v[2]);
            Assert::IsTrue(1 == v[3]);
            Assert::IsTrue(2 == v[4]);
            Assert::IsTrue(1 == v[5]);
            Assert::IsTrue(1 == v[6]);
            Assert::IsTrue(1 == v[7]);
            Assert::IsTrue(1 == v[8]);
            Assert::IsTrue(1 == v[9]);
            Assert::IsTrue(1 == v[10]);
            Assert::IsTrue(1 == v[11]);
            Assert::IsTrue(1 == v[12]);
            Assert::IsTrue(1 == v[13]);
            Assert::IsTrue(4 == v[14]);
            Assert::IsTrue(3 == v[15]);
            Assert::IsTrue(3 == v[16]);
            Assert::IsTrue(2 == v[17]);
            Assert::IsTrue(2 == v[18]);
            Assert::IsTrue(2 == v[19]);
            Assert::IsTrue(2 == v[20]);
            Assert::IsTrue(2 == v[21]);


        }
	};
}