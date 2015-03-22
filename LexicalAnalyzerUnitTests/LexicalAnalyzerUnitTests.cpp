#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <LexicalAnalyzer\LexicalAnalyzer.h>
#include <LexicalAnalyzer\ILanguageInfo.h>
#include <LexicalAnalyzer\LanguageInfoCpp.h>

using namespace LexicalAnalysis;

namespace LexicalAnalyzerUnitTests
{
    TEST_CLASS(LexicalAnalyzerUnitTests)
    {
    public:

        TEST_METHOD(ShouldCreateLexicalAnalyzer)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);
        }

        TEST_METHOD(ShouldParseKeywordCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_KEYWORD);
        }

        TEST_METHOD(ShouldParseKeywordCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("const_cast");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_KEYWORD);
        }

        TEST_METHOD(ShouldParseKeywordCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("constexpr");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_KEYWORD);
        }

        TEST_METHOD(ShouldParseIndentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("abcd123");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseIndentifierCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("_1fesaofkeasopkfpoesakfopkesaopfk3");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseSeparatorCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("[");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_SEPARATOR);
        }

        TEST_METHOD(ShouldParseSeparatorCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("}");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0];
            Assert::IsTrue(lexeme_type == LanguageInfoCpp::LC_SEPARATOR);
        }

        TEST_METHOD(ShouldParseKeywordSeparatorIdentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int}indentifier1");
            Assert::IsTrue(res.size() == 3);

            auto lexeme_type1 = res[0];
            Assert::IsTrue(lexeme_type1 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type2 = res[1];
            Assert::IsTrue(lexeme_type2 == LanguageInfoCpp::LC_SEPARATOR);


            auto lexeme_type3 = res[2];
            Assert::IsTrue(lexeme_type3 == LanguageInfoCpp::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseKeywordSpaceIdentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int indentifier1");
            Assert::IsTrue(res.size() == 2);

            auto lexeme_type1 = res[0];
            Assert::IsTrue(lexeme_type1 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type2 = res[1];
            Assert::IsTrue(lexeme_type2 == LanguageInfoCpp::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseWithSpacesCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int const constexpr\nabcd1   \tabc static_cast{const}");
            Assert::IsTrue(res.size() == 9);

            auto lexeme_type1 = res[0];
            Assert::IsTrue(lexeme_type1 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type2 = res[1];
            Assert::IsTrue(lexeme_type2 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type3 = res[2];
            Assert::IsTrue(lexeme_type3 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type4 = res[3];
            Assert::IsTrue(lexeme_type4 == LanguageInfoCpp::LC_IDENTIFIER);

            auto lexeme_type5 = res[4];
            Assert::IsTrue(lexeme_type5 == LanguageInfoCpp::LC_IDENTIFIER);

            auto lexeme_type6 = res[5];
            Assert::IsTrue(lexeme_type6 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type7 = res[6];
            Assert::IsTrue(lexeme_type7 == LanguageInfoCpp::LC_SEPARATOR);
            
            auto lexeme_type8 = res[7];
            Assert::IsTrue(lexeme_type8 == LanguageInfoCpp::LC_KEYWORD);

            auto lexeme_type9 = res[8];
            Assert::IsTrue(lexeme_type9 == LanguageInfoCpp::LC_SEPARATOR);
        }

        TEST_METHOD(ShouldParseIntegerCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("123456");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_INTEGER_LITERAL);
        }


        TEST_METHOD(ShouldParseIntegerCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("1543534ul 12321ull 325uLl 12331uLL 23432ulL 123llu 123LLu 1232Llu 214lLu");
            Assert::IsTrue(res.size() == 9);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[1] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[2] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[3] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[4] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[5] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[6] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[7] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[8] == LanguageInfoCpp::LC_INTEGER_LITERAL);
        }


        TEST_METHOD(ShouldParseIntegerCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            {
                auto res = cpp_analyzer.ParseString("012312147");
                Assert::IsTrue(res.size() == 1);
                Assert::IsTrue(res[0] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            }

            {
                auto res = cpp_analyzer.ParseString("05");
                Assert::IsTrue(res.size() == 1);
                Assert::IsTrue(res[0] == LanguageInfoCpp::LC_INTEGER_LITERAL);
            }
        }


        TEST_METHOD(ShouldParseIntegerCase4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("0xAb123ac");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_INTEGER_LITERAL);
        }


        TEST_METHOD(ShouldAcceptCharacherLiteralCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'a\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'3\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'\\n\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'\\123\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase5)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'\\xabc\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("123.123");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("1.e+1");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString(".123e+10");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("123.e-5");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase5)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("3.14159265");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase6)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("2.34E+22f");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralIntegralCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("468.0f 2.34E+22f 3.14159265 3.14159265f 12.5e9f 1.0f 1.0f 1.234f 2.345234E28L 2.34E+22f");
            Assert::IsTrue(res.size() == 10);
            Assert::IsTrue(res[0] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[1] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[2] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[3] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[4] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[5] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[6] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[7] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[8] == LanguageInfoCpp::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[9] == LanguageInfoCpp::LC_FLOATING_LITERAL);
        }

    };

    
}