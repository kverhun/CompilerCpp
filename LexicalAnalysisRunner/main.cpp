#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <LexicalAnalyzer\FileIO.h>
#include <LexicalAnalyzer\LanguageInfoCpp.h>
#include <LexicalAnalyzer\LexicalAnalyzer.h>

#include <SyntaxAnalyzer\SyntaxAnalyzer.h>
#include <SyntaxAnalyzer\GrammarGenerators.h>
#include <SyntaxAnalyzer\SyntaxAnalysisHelpers.h>

namespace
{
    std::vector<size_t> _GetNewLinePositions(const std::string& i_str)
    {
        std::vector<size_t> new_lines;
        for (size_t i = 0; i < i_str.size(); ++i)
        {
            if (i_str[i] == '\n')
                new_lines.push_back(i);
        }

        return new_lines;
    }

    std::pair<size_t, size_t> _GetLineAndPosNumber(size_t i_pos, const std::vector<size_t> i_new_lines)
    {
        size_t line_number = 0;
        while (line_number < i_new_lines.size() - 1 && i_pos > i_new_lines[line_number+1])
            ++line_number;
        return std::make_pair(line_number + 1, i_pos - i_new_lines[line_number]);
    }
}

int main(int i_argc, const char** i_argv)
{
    if (i_argc < 2)
    {
        std::cout << "Please specify file name for parsing" << std::endl;
        return 0;
    }

    std::stringstream out_stream;
    
    std::string input_fname = i_argv[1];
    
    auto input_string = LexicalAnalysis::FileIO::GetFileString(input_fname);
    input_string.insert(0, 1, '\n');

    //std::cout << "string to parse: " << std::endl << input_string << std::endl;
    LexicalAnalysis::LanguageInfoCpp langinfo;
    LexicalAnalysis::LexicalAnalyzer analyzer(langinfo);
    auto parsed_file = analyzer.ParseString(input_string);
    
    auto p_grammar = SyntaxAnalysis::GenerateGrammarCpp();
    auto syntax_analyzer = SyntaxAnalysis::SyntaxAnalyzer(*p_grammar);
    
    auto parsed_file_for_as = SyntaxAnalysis::SyntaxAnalysisHelpers::FixParsedStringForCpp(parsed_file);
    std::vector<size_t> productions_used;
    bool syntax_success = syntax_analyzer.Analyze(productions_used, parsed_file_for_as);

    size_t col1_width = 30;
    size_t col2_width = 30;
    size_t col3_widht = 20;

    auto new_lines = _GetNewLinePositions(input_string);
    
    out_stream << "======== LEXICAL ANALYSIS ============" << std::endl;

    for (auto lexeme : parsed_file)
    {
        std::string lexeme_type = langinfo.GetLexemeClasses().find(lexeme.m_lexeme_class)->second;
        std::string lexeme_value = lexeme.m_lexeme_value;
        size_t lexeme_position = lexeme.m_lexeme_position;

        auto lexeme_file_position = _GetLineAndPosNumber(lexeme_position, new_lines);

        std::string first_separator(col1_width > lexeme_value.size() ? col1_width - lexeme_value.size() : 1, ' ');
        std::string second_separator(col2_width > lexeme_type.size() ? col2_width - lexeme_type.size()  : 1, ' ');
        out_stream << lexeme_value << first_separator << lexeme_type << second_separator << "l: " << lexeme_file_position.first << ", c: " << lexeme_file_position.second << std::endl;
    }

    auto derivation = SyntaxAnalysis::SyntaxAnalysisHelpers::GetDerivation(*p_grammar, productions_used);

    out_stream << "========= SYNTAX ANALYSIS ============" << std::endl;
    if (syntax_success)
    {
        out_stream << "SUCCEED" << std::endl;
        out_stream << "Productions used: " << std::endl;
        out_stream << "[";
        for (size_t i = 0; i + 1 < productions_used.size(); ++i)
            out_stream << productions_used[i] << "; ";
        out_stream << *productions_used.rbegin() << "]";

        out_stream << std::endl << "Derivation:" << std::endl;

        for (auto symbol_sequence : derivation)
        {
            for (auto symbol : symbol_sequence)
                out_stream << symbol.ToString() << " ";

            out_stream << std::endl;
        }
    }
    else
    {
        out_stream << "FAILED" << std::endl;
    }

    if (i_argc > 2)
    {
        std::string out_file = i_argv[2];
        std::ofstream out_file_stream(out_file);
        out_file_stream << out_stream.str();
    }
    else
    {
        std::cout << out_stream.str();
    }

    return 0;
}