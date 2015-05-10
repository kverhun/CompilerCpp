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

            g(start_symbol, { {GrammarSymbol(terminal_1)}, {GrammarSymbol(terminal_1), GrammarSymbol(terminal_2), start_symbol} });

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

            g(start_symbol, { { GrammarSymbol(terminal_1) }, { GrammarSymbol(terminal_1), GrammarSymbol(terminal_2), start_symbol }, { GrammarSymbol(GrammarSymbol::GST_LAMBDA) } });

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


	};
}