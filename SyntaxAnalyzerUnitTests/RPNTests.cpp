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
#include <SyntaxAnalyzer\ParseTree.h>
#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>
#include <Interfaces\SyntaxAnalyzer\SyntaxAnalyzerInput.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace SyntaxAnalysis;

namespace SyntaxAnalyzerUnitTests
{
	TEST_CLASS(RPNTests)
	{
	public:
		
		TEST_METHOD(AddMultiplyTest1)
		{
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                 a + b * c; \
                 }";
            
                    
            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "c");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "*");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "end");

		}

        TEST_METHOD(AddMultiplyTest2)
		{
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                 (a + b) * c; \
                 }";
            
                    
            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "c");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "*");
            Assert::IsTrue(*it++ == "end");

		}

        TEST_METHOD(AssingmentTest1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                a = b; \
            }";


            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);
            Assert::AreEqual(size_t{ 5 }, rpn.size());

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "end");

        }

        TEST_METHOD(AssingmentTest2)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                a = b + c; \
            }";


            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);
            
            Assert::AreEqual(size_t{ 7 }, rpn.size());

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "c");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "end");

        }

        TEST_METHOD(BlockTest1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                 {\
                 a = b + c; \
                 }\
                 {\
                 d = e + f;\
                 h = g + i;\
                 }\
                 }";


            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);

            //Assert::AreEqual(size_t{ 7 }, rpn.size());

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "c");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "end");

            Assert::IsTrue(*it++ == "begin");
            
            Assert::IsTrue(*it++ == "f");
            Assert::IsTrue(*it++ == "e");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "d");
            Assert::IsTrue(*it++ == "=");

            Assert::IsTrue(*it++ == "i");
            Assert::IsTrue(*it++ == "g");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "h");
            Assert::IsTrue(*it++ == "=");

            Assert::IsTrue(*it++ == "end");
            Assert::IsTrue(*it++ == "end");

        }

        TEST_METHOD(IfTest1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                 if (a > b) a = b;else b = a; \
                 }";


            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == ">");
            Assert::IsTrue(*it++ == "m1");
            Assert::IsTrue(*it++ == "IFFALSE");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "m2");
            Assert::IsTrue(*it++ == "GOTO");
            Assert::IsTrue(*it++ == "m1:");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "m2:");
            Assert::IsTrue(*it++ == "end");

        }

        TEST_METHOD(IfTest2)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                 if (a > b) a = b; \
                 }";


            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == ">");
            Assert::IsTrue(*it++ == "m1");
            Assert::IsTrue(*it++ == "IFFALSE");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "m1:");
            Assert::IsTrue(*it++ == "end");
        }

        TEST_METHOD(WhileTest1)
        {
            typedef LexicalAnalysis::TParsedString TParsedString;
            using LexicalAnalysis::LexemeInfo;
            using LexicalAnalysis::LanguageInfoCpp;
            using LexicalAnalysis::LexicalAnalyzer;

            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string =
                "int main (){ \
                 while (a > b) a = a + 1; \
                 }";


            auto res = cpp_analyzer.ParseString(string);
            auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();

            SyntaxAnalysis::SyntaxAnalyzer sa(*p_grammar);
            std::vector<size_t> v;
            bool sa_res = sa.Analyze(v, SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(res));

            ParseTree pt(*p_grammar, v);
            auto rpn = pt.GetReversePolishNotation(res);

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "begin");
            Assert::IsTrue(*it++ == "m1:");
            Assert::IsTrue(*it++ == "1");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == ">");
            Assert::IsTrue(*it++ == "m2");
            Assert::IsTrue(*it++ == "IFFALSE");
            Assert::IsTrue(*it++ == "m1");
            Assert::IsTrue(*it++ == "GOTO");
            Assert::IsTrue(*it++ == "m2:");
            Assert::IsTrue(*it++ == "end");
        }

	};
}