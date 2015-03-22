#include <iostream>

#include <string>
#include <LexicalAnalyzer\FileIO.h>
#include <LexicalAnalyzer\LanguageInfoCpp.h>
#include <LexicalAnalyzer\LexicalAnalyzer.h>

int main(int i_argc, const char** i_argv)
{
    if (i_argc < 2)
    {
        std::cout << "Please specify file name for parsing" << std::endl;
        return 0;
    }

    std::string input_fname = i_argv[1];
    auto input_string = LexicalAnalysis::FileIO::GetFileString(input_fname);

    //std::cout << "string to parse: " << std::endl << input_string << std::endl;
    LexicalAnalysis::LanguageInfoCpp langinfo;
    LexicalAnalysis::LexicalAnalyzer analyzer(langinfo);
    auto parsed_file = analyzer.ParseString(input_string);
    
    size_t col1_width = 30;
    size_t col2_width = 30;
    size_t col3_widht = 20;

    for (auto lexeme : parsed_file)
    {
        std::string lexeme_type = langinfo.GetLexemeClasses().find(lexeme.m_lexeme_class)->second;
        std::string lexeme_value = lexeme.m_lexeme_value;
        size_t lexeme_position = lexeme.m_lexeme_position;

        std::string first_separator(col1_width > lexeme_value.size() ? col1_width - lexeme_value.size() : 1, ' ');
        std::string second_separator(col2_width > lexeme_type.size() ? col2_width - lexeme_type.size()  : 1, ' ');
        std::cout << lexeme_value << first_separator << lexeme_type << second_separator << lexeme_position << std::endl;
    }

    return 0;
}