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
            Assert::IsTrue(*it++ == "c");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "*");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "+");

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
            Assert::IsTrue(*it++ == "c");
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "+");
            Assert::IsTrue(*it++ == "*");

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
            Assert::AreEqual(size_t{ 3 }, rpn.size());

            auto it = rpn.begin();
            Assert::IsTrue(*it++ == "b");
            Assert::IsTrue(*it++ == "a");
            Assert::IsTrue(*it++ == "=");

        }

	};
}