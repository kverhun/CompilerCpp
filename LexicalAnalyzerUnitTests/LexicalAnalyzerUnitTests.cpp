#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <LexicalAnalyzer\LexicalAnalyzer.h>
#include <Interfaces\LexicalAnalyzer\ILanguageInfo.h>
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

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_KEYWORD);
        }

        TEST_METHOD(ShouldParseKeywordCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("const_cast");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_KEYWORD);
        }

        TEST_METHOD(ShouldParseKeywordCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("constexpr");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_KEYWORD);
        }

        TEST_METHOD(ShouldParseIndentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("abcd123");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseIndentifierCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("_1fesaofkeasopkfpoesakfopkesaopfk3");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseSeparatorCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("[");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
        }

        TEST_METHOD(ShouldParseSeparatorCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("}");
            Assert::IsTrue(res.size() == 1);

            auto lexeme_type = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
        }

        TEST_METHOD(ShouldParseKeywordSeparatorIdentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int}indentifier1");
            Assert::IsTrue(res.size() == 3);

            auto lexeme_type1 = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type1 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type2 = res[1].m_lexeme_class;
            Assert::IsTrue(lexeme_type2 == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);

            auto lexeme_type3 = res[2].m_lexeme_class;
            Assert::IsTrue(lexeme_type3 == ECppLexemeClasses::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseKeywordSpaceIdentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int indentifier1");
            Assert::IsTrue(res.size() == 2);

            auto lexeme_type1 = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type1 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type2 = res[1].m_lexeme_class;
            Assert::IsTrue(lexeme_type2 == ECppLexemeClasses::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldParseWithSpacesCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("int const constexpr\nabcd1   \tabc static_cast{const}");
            Assert::IsTrue(res.size() == 9);

            auto lexeme_type1 = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type1 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type2 = res[1].m_lexeme_class;
            Assert::IsTrue(lexeme_type2 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type3 = res[2].m_lexeme_class;
            Assert::IsTrue(lexeme_type3 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type4 = res[3].m_lexeme_class;
            Assert::IsTrue(lexeme_type4 == ECppLexemeClasses::LC_IDENTIFIER);

            auto lexeme_type5 = res[4].m_lexeme_class;
            Assert::IsTrue(lexeme_type5 == ECppLexemeClasses::LC_IDENTIFIER);

            auto lexeme_type6 = res[5].m_lexeme_class;
            Assert::IsTrue(lexeme_type6 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type7 = res[6].m_lexeme_class;
            Assert::IsTrue(lexeme_type7 == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            
            auto lexeme_type8 = res[7].m_lexeme_class;
            Assert::IsTrue(lexeme_type8 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type9 = res[8].m_lexeme_class;
            Assert::IsTrue(lexeme_type9 == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
        }

        TEST_METHOD(ShouldParseWithSpacesCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("const}");
            Assert::IsTrue(res.size() == 2);

            auto lexeme_type8 = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type8 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type9 = res[1].m_lexeme_class;
            Assert::IsTrue(lexeme_type9 == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
        }

        TEST_METHOD(ShouldParseWithSpacesCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("{const}");
            Assert::IsTrue(res.size() == 3);

            auto lexeme_type7 = res[0].m_lexeme_class;
            Assert::IsTrue(lexeme_type7 == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);

            auto lexeme_type8 = res[1].m_lexeme_class;
            Assert::IsTrue(lexeme_type8 == ECppLexemeClasses::LC_KEYWORD);

            auto lexeme_type9 = res[2].m_lexeme_class;
            Assert::IsTrue(lexeme_type9 == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
        }

        TEST_METHOD(ShouldParseIntegerCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("123456");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
        }


        TEST_METHOD(ShouldParseIntegerCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("1543534ul 12321ull 325uLl 12331uLL 23432ulL 123llu 123LLu 1232Llu 214lLu");
            Assert::IsTrue(res.size() == 9);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[1].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[2].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[3].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[4].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[5].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[6].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[7].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            Assert::IsTrue(res[8].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
        }


        TEST_METHOD(ShouldParseIntegerCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            {
                auto res = cpp_analyzer.ParseString("012312147");
                Assert::IsTrue(res.size() == 1);
                Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            }

            {
                auto res = cpp_analyzer.ParseString("05");
                Assert::IsTrue(res.size() == 1);
                Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
            }
        }


        TEST_METHOD(ShouldParseIntegerCase4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("0xAb123ac");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_INTEGER_LITERAL);
        }


        TEST_METHOD(ShouldAcceptCharacherLiteralCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'a\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'3\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'\\n\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'\\123\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptCharacherLiteralCase5)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\'\\xabc\'");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_CHARACTER_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("123.123");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("1.e+1");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString(".123e+10");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("123.e-5");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase5)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("3.14159265");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralCase6)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("2.34E+22f");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }

        TEST_METHOD(ShouldNotAcceptInvalidFloatCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("2.34E+X");
            Assert::IsTrue(res[0].m_lexeme_class == ILanguageInfo::ERROR_LEXEME);
        }

        TEST_METHOD(ShouldNotAcceptInvalidFloatCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("2.34E+X;");
            Assert::IsTrue(res[0].m_lexeme_class == ILanguageInfo::ERROR_LEXEME);
        }

        TEST_METHOD(ShouldNotAcceptInvalidIndentifierCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("2d3d");
            Assert::IsTrue(res[0].m_lexeme_class == ILanguageInfo::ERROR_LEXEME);
        }

        TEST_METHOD(ShouldNotAcceptInvalidIndentifierCase2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("0dc\n2d3d");
            Assert::IsTrue(res[0].m_lexeme_class == ILanguageInfo::ERROR_LEXEME);
        }

        TEST_METHOD(ShouldAcceptFloatingLiteralIntegralCase1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("468.0f 2.34E+22f 3.14159265 3.14159265f 12.5e9f 1.0f 1.0f 1.234f 2.345234E28L 2.34E+22f");
            Assert::IsTrue(res.size() == 10);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[1].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[2].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[3].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[4].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[5].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[6].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[7].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[8].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[9].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
        }


        TEST_METHOD(ShouldAcceptStringLiteral1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\"abcd\"");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_STRING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptStringLiteral2)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\"abcd123\\n\\a\\tawefaew\\a\"");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_STRING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptStringLiteral3)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\"hello world\"");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_STRING_LITERAL);
        }

        TEST_METHOD(ShouldAcceptStringLiteral4)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("\"Please specify file name for parsing\"");
            Assert::IsTrue(res.size() == 1);
            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_STRING_LITERAL);
        }

        TEST_METHOD(TestParenIssue01)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("(a");
            Assert::IsTrue(res.size() == 2);
            //Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_STRING_LITERAL);
        }

        TEST_METHOD(TestParenIssue02)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto res = cpp_analyzer.ParseString("a (a");
            Assert::IsTrue(res.size() == 3);
            //Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_STRING_LITERAL);
        }

        TEST_METHOD(ComplexTest1)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string = "const auto& automat : m_lang_info.GetLexemeAutomataPoolMap().find(lexeme_type)->second";
            auto res = cpp_analyzer.ParseString(string);
            Assert::IsTrue(17 == res.size());

            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_KEYWORD);
            Assert::IsTrue(res[1].m_lexeme_class == ECppLexemeClasses::LC_KEYWORD);
            Assert::IsTrue(res[2].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[3].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
            Assert::IsTrue(res[4].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[5].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
            Assert::IsTrue(res[6].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[7].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
            Assert::IsTrue(res[8].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[9].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[10].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[11].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
            Assert::IsTrue(res[12].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[13].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
            Assert::IsTrue(res[14].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[15].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[16].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
        }

        TEST_METHOD(ShouldReturnLexemeValueAndPosition)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto string = "const float a = 1.123e+24;";
            auto res = cpp_analyzer.ParseString(string);
            Assert::IsTrue(res.size() == 6);

            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_KEYWORD);
            Assert::IsTrue(res[0].m_lexeme_value == "const");
            Assert::IsTrue(res[0].m_lexeme_position == 0);

            Assert::IsTrue(res[1].m_lexeme_class == ECppLexemeClasses::LC_KEYWORD);
            Assert::IsTrue(res[1].m_lexeme_value == "float");
            Assert::IsTrue(res[1].m_lexeme_position == 6);

            Assert::IsTrue(res[2].m_lexeme_class == ECppLexemeClasses::LC_IDENTIFIER);
            Assert::IsTrue(res[2].m_lexeme_value == "a");
            Assert::IsTrue(res[2].m_lexeme_position == 12);

            Assert::IsTrue(res[3].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[3].m_lexeme_value == "=");
            Assert::IsTrue(res[3].m_lexeme_position == 14);

            Assert::IsTrue(res[4].m_lexeme_class == ECppLexemeClasses::LC_FLOATING_LITERAL);
            Assert::IsTrue(res[4].m_lexeme_value == "1.123e+24");
            Assert::IsTrue(res[4].m_lexeme_position == 16);

            Assert::IsTrue(res[5].m_lexeme_class == ECppLexemeClasses::LC_PUNCTUATION_OR_OPERATOR);
            Assert::IsTrue(res[5].m_lexeme_value == ";");
            Assert::IsTrue(res[5].m_lexeme_position == 25);
        }

        TEST_METHOD(ShouldParseBooleanLiterals)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto str = "true false";
            auto res = cpp_analyzer.ParseString(str);
            Assert::IsTrue(res.size() == 2);

            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_BOOLEAN_LITERAL);
            Assert::IsTrue(res[0].m_lexeme_value == "true");
            Assert::IsTrue(res[0].m_lexeme_position == 0);

            Assert::IsTrue(res[1].m_lexeme_class == ECppLexemeClasses::LC_BOOLEAN_LITERAL);
            Assert::IsTrue(res[1].m_lexeme_value == "false");
            Assert::IsTrue(res[1].m_lexeme_position == 5);
        }

        TEST_METHOD(ShouldParsePointerLiteral)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto str = "nullptr";
            auto res = cpp_analyzer.ParseString(str);
            Assert::IsTrue(res.size() == 1);

            Assert::IsTrue(res[0].m_lexeme_class == ECppLexemeClasses::LC_POINTER_LITERAL);
            Assert::IsTrue(res[0].m_lexeme_value == "nullptr");
            Assert::IsTrue(res[0].m_lexeme_position == 0);
        }

        TEST_METHOD(ShouldNotParseIllegalString)
        {
            LanguageInfoCpp langinfo;
            LexicalAnalyzer cpp_analyzer(langinfo);

            auto str = "\"abc\\oabc\"";
            auto res = cpp_analyzer.ParseString(str);
            Assert::IsTrue(res.size() == 1);

            Assert::IsTrue(res[0].m_lexeme_class == ILanguageInfo::ERROR_LEXEME);
            Assert::IsTrue(res[0].m_lexeme_value == "\"abc\\oabc\"");
            Assert::IsTrue(res[0].m_lexeme_position == 0);
        }


    };

    
}