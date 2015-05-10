#include "stdafx.h"
#include "CppUnitTest.h"

#include <LexicalAnalyzer\LexicalAnalyzer.h>
#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
#include <LexicalAnalyzer\LanguageInfoCpp.h>
#include <SyntaxAnalyzer\SyntaxAnalyzer.h>
#include <SyntaxAnalyzer\Grammar.h>
#include <SyntaxAnalyzer\GrammarGenerators.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

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

            //TParsedString actual = { terminal_if_lexeme, terminal_lexeme, terminal_left_brace_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme, terminal_right_brace_lexeme, terminal_else_lexeme, terminal_lexeme, terminal_add_op1_lexeme, terminal_lexeme, terminal_semicolon_lexeme };
            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            Assert::IsTrue(sa.Analyze(res));

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

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            Assert::IsTrue(sa.Analyze(res));

        }

        TEST_METHOD(SALAShouldReturnCorrectProductionSeq1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "ID;";

            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

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

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, res);

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