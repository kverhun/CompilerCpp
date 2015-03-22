#include "LanguageInfoCpp.h"

#include "DFA.h"
#include "DFACreationHelpers.h"

using namespace LexicalAnalysis;

//------------------------------------------------------------------------------
typedef ILanguageInfo::TLexemeAutomataPoolMap TLexemeAutomataPoolMap;
typedef ILanguageInfo::TLexemeClassPriority TLexemeClassPriority;
typedef ILanguageInfo::TLexemeClasses TLexemeClasses;
typedef ILanguageInfo::TAlphabet TAlphabet;

//------------------------------------------------------------------------------
LanguageInfoCpp::LanguageInfoCpp()
{
    _CreateAlphabet();
    _CreateLexemeClasses();
    _CreateLexemeAutomataPoolMap();
}

//------------------------------------------------------------------------------
LanguageInfoCpp::~LanguageInfoCpp()
{

}

//------------------------------------------------------------------------------
const TLexemeAutomataPoolMap& LanguageInfoCpp::GetLexemeAutomataPoolMap() const
{
    return m_lexeme_automata_pool_map;
}

//------------------------------------------------------------------------------
const TLexemeClassPriority& LanguageInfoCpp::GetLexemeClassPriority() const
{
    return m_lexeme_class_priority;
}

//------------------------------------------------------------------------------
const TLexemeClasses& LanguageInfoCpp::GetLexemeClasses() const
{
    return m_lexeme_classes;
}

//------------------------------------------------------------------------------
const TAlphabet& LanguageInfoCpp::GetAlphabet() const
{
    return m_alphabet;
}

//------------------------------------------------------------------------------
void LanguageInfoCpp::_CreateAlphabet()
{
	// letters
    for (char ch = 'a'; ch <= 'z'; ++ch)
    {
        m_alphabet.push_back(ch);
        m_letters.push_back(ch);
    }
    for (char ch = 'A'; ch <= 'Z'; ++ch)
    {
        m_alphabet.push_back(ch);
        m_letters.push_back(ch);
    }

	m_alphabet.push_back('_');
    m_letters.push_back('_');

	// digits
    for (char ch = '0'; ch <= '9'; ++ch)
    {
        m_alphabet.push_back(ch);
        m_digits.push_back(ch);
        if (ch != '0')
            m_nonzero_digits.push_back(ch);
    }

	// operators and separators
	
    m_alphabet.push_back('(');
    m_alphabet.push_back(')');
    m_alphabet.push_back('{');
    m_alphabet.push_back('}');
    m_alphabet.push_back('[');
    m_alphabet.push_back(']');
    m_alphabet.push_back('+');
    m_alphabet.push_back('-');
    m_alphabet.push_back('*');
    m_alphabet.push_back('/');
    m_alphabet.push_back('%');
    m_alphabet.push_back('=');
    
    m_separators.push_back('(');
    m_separators.push_back(')');
    m_separators.push_back('{');
    m_separators.push_back('}');
    m_separators.push_back('[');
    m_separators.push_back(']');
    m_separators.push_back('+');
    m_separators.push_back('-');
    m_separators.push_back('*');
    m_separators.push_back('/');
    m_separators.push_back('%');
    m_separators.push_back('=');

    m_octal_digits = { '0', '1', '2', '3', '4', '5', '6', '7' };
    m_hexadecimal_digits = { 
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F' 
    };

    m_c_char = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '_',
        '(', ')', '{', '}', '[', ']', '+', '-', '*', '/', '%', '=', ' '
    };

}

//------------------------------------------------------------------------------
void LanguageInfoCpp::_CreateLexemeClasses()
{
	m_lexeme_classes[LC_IDENTIFIER] = "Identifier";
	m_lexeme_classes[LC_KEYWORD] = "Keyword";
    m_lexeme_classes[LC_SEPARATOR] = "Separator";
    m_lexeme_classes[LC_INTEGER_LITERAL] = "Integer literal";
    m_lexeme_classes[LC_CHARACTER_LITERAL] = "Character literal";
    m_lexeme_classes[LC_FLOATING_LITERAL] = "Floating literal";

    m_lexeme_classes[LC_ERROR] = "Error";

    m_lexeme_class_priority = { 
        LC_KEYWORD, 
        LC_IDENTIFIER, 
        LC_SEPARATOR, 
        LC_INTEGER_LITERAL,
        LC_CHARACTER_LITERAL,
        LC_FLOATING_LITERAL,
        LC_ERROR
    };
}

//------------------------------------------------------------------------------
void LanguageInfoCpp::_CreateLexemeAutomataPoolMap()
{
    // 1. LC_IDENTIFIER
    DFA identifier_dfa;
    identifier_dfa.AddState(0, DFA::ST_INTERMEDIATE);
    identifier_dfa.AddState(1, DFA::ST_TERMINAL_ACCEPTED);
    identifier_dfa.AddTransition(0, m_letters, 1);
    identifier_dfa.AddTransition(1, m_letters, 1);
    identifier_dfa.AddTransition(1, m_digits, 1);

    m_lexeme_automata_pool_map[LC_IDENTIFIER] = { identifier_dfa };

    // 2. LC_KEYWORD
    std::vector<std::string> keywords = {
        "alignas", "alignof", "asm", "auto", "bool", "break", "case", "catch", "char",
        "char16_t", "char32_t", "class", "const", "constexpr", "const_cast", "continue",
        "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
        "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline",
        "int", "long",
        "mutable", "namespace", "new", "noexcept", "nullptr", "operator",
        "private", "protected", "public", "register", "reinterpret_cast", "return",
        "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
        "switch", "template", "this", "thread_local", "throw", "true", "try", "typedef",
        "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile",
        "wchar_t", "while"
    };

    TAutomataPool keywords_automata_pool;
    keywords_automata_pool.reserve(keywords.size());
    for (auto keyword : keywords)
        keywords_automata_pool.push_back(DFACreationHelpers::CreateForString(keyword));
    m_lexeme_automata_pool_map[LC_KEYWORD] = std::move(keywords_automata_pool);

    // 3. LC_SEPARATOR
    DFA separator_dfa;
    separator_dfa.AddState(0, DFA::ST_INTERMEDIATE);
    separator_dfa.AddState(1, DFA::ST_TERMINAL_ACCEPTED);
    separator_dfa.SetStartState(0);
    separator_dfa.AddTransition(0, m_separators, 1);
    m_lexeme_automata_pool_map[LC_SEPARATOR] = { separator_dfa };

    // 5. LC_INTEGER_LITERAL
    DFA integer_literal_dfa_decimal;
    integer_literal_dfa_decimal.AddState(0, DFA::ST_INTERMEDIATE);
    integer_literal_dfa_decimal.AddState(1, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_dfa_decimal.AddState(2, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_dfa_decimal.AddState(3, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_dfa_decimal.AddState(4, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_dfa_decimal.AddState(5, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_dfa_decimal.AddState(6, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_dfa_decimal.SetStartState(0);
    integer_literal_dfa_decimal.AddTransition(0, m_nonzero_digits, 1);
    integer_literal_dfa_decimal.AddTransition(1, m_digits, 1);
    integer_literal_dfa_decimal.AddTransition(1, { 'u', 'U' }, 2);
    integer_literal_dfa_decimal.AddTransition(2, { 'l', 'L' }, 3);
    integer_literal_dfa_decimal.AddTransition(3, { 'l', 'L' }, 4);
    integer_literal_dfa_decimal.AddTransition(1, { 'l', 'L' }, 5);
    integer_literal_dfa_decimal.AddTransition(5, { 'u', 'U' }, 4);
    integer_literal_dfa_decimal.AddTransition(5, { 'l', 'L' }, 6);
    integer_literal_dfa_decimal.AddTransition(6, { 'u', 'U' }, 4);

    DFA integer_literal_octal;
    integer_literal_octal.AddState(0, DFA::ST_INTERMEDIATE);
    integer_literal_octal.AddState(1, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_octal.AddTransition(0, '0', 1);
    integer_literal_octal.AddTransition(1, m_octal_digits, 1);
    integer_literal_octal.SetStartState(0);

    DFA integer_literal_hexadecimal;
    integer_literal_hexadecimal.AddState(0, DFA::ST_INTERMEDIATE);
    integer_literal_hexadecimal.AddState(1, DFA::ST_INTERMEDIATE);
    integer_literal_hexadecimal.AddState(2, DFA::ST_INTERMEDIATE);
    integer_literal_hexadecimal.AddState(3, DFA::ST_TERMINAL_ACCEPTED);
    integer_literal_hexadecimal.AddTransition(0, '0', 1);
    integer_literal_hexadecimal.AddTransition(1, { 'x', 'X' }, 2);
    integer_literal_hexadecimal.AddTransition(2, m_hexadecimal_digits, 3);
    integer_literal_hexadecimal.AddTransition(3, m_hexadecimal_digits, 3);

    m_lexeme_automata_pool_map[LC_INTEGER_LITERAL] = { integer_literal_dfa_decimal, integer_literal_octal, integer_literal_hexadecimal };

    // 6. LC_CHARACTER_LITERAL
    DFA character_literal_dfa;
    character_literal_dfa
        (0, DFA::ST_INTERMEDIATE)(1, DFA::ST_INTERMEDIATE)(2, DFA::ST_INTERMEDIATE)(3, DFA::ST_INTERMEDIATE)
        (4, DFA::ST_TERMINAL_ACCEPTED)(5, DFA::ST_INTERMEDIATE)(6, DFA::ST_INTERMEDIATE)(7, DFA::ST_INTERMEDIATE)
        (8, DFA::ST_INTERMEDIATE)(9, DFA::ST_INTERMEDIATE)(10, DFA::ST_INTERMEDIATE)(11, DFA::ST_INTERMEDIATE);
    character_literal_dfa
        (0, { 'u', 'U', 'L' }, 1)(0, '\'', 2)(1, '\'', 2)(2, m_c_char, 3)(2, '\\', 5)(3, m_c_char, 3)(3, '\'', 4)
        (5, {'\'', '\"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v'}, 11)(11, '\'', 4)
        (5, m_octal_digits, 6)(6, m_octal_digits, 7)(7, m_octal_digits, 8)
        (6, '\'', 4)(7, '\'', 4)(8, '\'', 4)
        (5, 'x', 9)(9, m_hexadecimal_digits, 10)(10, m_hexadecimal_digits, 10)(10, '\'', 4);
    character_literal_dfa.SetStartState(0);
    m_lexeme_automata_pool_map[LC_CHARACTER_LITERAL] = { character_literal_dfa };
    

    // 7. LC_FLOATING_LITERAL
    DFA floating_literal_dfa;
    floating_literal_dfa
        (0, DFA::ST_INTERMEDIATE)(1, DFA::ST_INTERMEDIATE)(2, DFA::ST_TERMINAL_ACCEPTED)(3, DFA::ST_INTERMEDIATE)
        (4, DFA::ST_INTERMEDIATE)(5, DFA::ST_INTERMEDIATE)(6, DFA::ST_TERMINAL_ACCEPTED)
        (8, DFA::ST_TERMINAL_ACCEPTED)(9, DFA::ST_TERMINAL_ACCEPTED);
    floating_literal_dfa
        (0, m_digits, 1)(1, m_digits, 1)(0, '.', 3)(1, '.', 2)(1, { 'e', 'E' }, 4)(2, { 'e', 'E' }, 4)(2, m_digits, 8)(2, { 'f', 'F', 'l', 'L' }, 9)
        (3, m_digits, 8)(8, { 'e', 'E' }, 4)(8, m_digits, 8)(8, { 'f', 'F', 'l', 'L' }, 9)
        (4, { '+', '-' }, 5)(4, m_digits, 6)(5, m_digits, 6)(6, m_digits, 6)(6, { 'f', 'F', 'l', 'L' }, 9);
    floating_literal_dfa.SetStartState(0);
    m_lexeme_automata_pool_map[LC_FLOATING_LITERAL] = { floating_literal_dfa };

    // 1024. LC_ERROR
    m_lexeme_automata_pool_map[LC_ERROR] = { DFACreationHelpers::CreateAccepingAll() };

}



