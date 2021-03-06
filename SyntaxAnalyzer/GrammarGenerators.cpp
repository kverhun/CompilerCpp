#include "GrammarGenerators.h"

#include "GrammarSymbol.h"
#include "Grammar.h"

#include <Interfaces\LexicalAnalyzer\LexemeInfo.h>

using namespace SyntaxAnalysis;
using LexicalAnalysis::LexemeInfo;

//------------------------------------------------------------------------------
std::unique_ptr<Grammar> SyntaxAnalysis::GenerateGrammarCpp()
{
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
    auto relational_expression = NonTerminal("relational-expression");
    auto relational_expression_ex = NonTerminal("relational-expression-ex");
    auto shift_expression = NonTerminal("shift-expression");
    auto shift_expression_ex = NonTerminal("shilf-expression-ex");
    auto additive_expression = NonTerminal("additive-expression");
    auto additive_expression_ex = NonTerminal("additive-expression-ex");
    auto multiplicative_expression = NonTerminal("multiplicative-expression");
    auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");
    auto unary_expression = NonTerminal("unary-expression");
    auto unary_expression_ex = NonTerminal("unary-expression-ex");
    auto unary_operator = NonTerminal("unary-operator");
    auto postfix_expression = NonTerminal("postfix-expression");
    auto postfix_expression_ex = NonTerminal("postfix-expression-ex");
    auto primary_expression = NonTerminal("primary-expression");
    auto expression = NonTerminal("expression");
    auto expression_optional = NonTerminal("expression-opt");
    auto expression_list = NonTerminal("expression-list");
    auto expression_list_optional = NonTerminal("expression-list-optional");
    auto expression_statement = NonTerminal("expression-statement");
    auto if_statement = NonTerminal("if-statement");
    auto while_statement = NonTerminal("while-statement");
    auto return_statement = NonTerminal("return-statement");
    auto statement = NonTerminal("statement");
    auto statement_sequence = NonTerminal("statement-sequence");
    auto statement_sequence_optional = NonTerminal("statement-sequence-opt");
    auto compound_statement = NonTerminal("compound-statement");
    
    auto identifier_list = NonTerminal("identifier-list");
    auto variable_definition = NonTerminal("variable-definition");
    auto parameter_declaration_list_optional = NonTerminal("parameter-declaration-list-opt");
    auto parameter_declaration = NonTerminal("parameter-declaration");
    auto parameter_declaration_list = NonTerminal("parameter-declaration-list");
    auto type_specifier = NonTerminal("type-specifier");
    auto function_declarator = NonTerminal("function-declarator");
    auto function_body = NonTerminal("function-body");
    auto function_definition = NonTerminal("function-definition");

    auto declaration = NonTerminal("declaration");
    auto declaration_sequence = NonTerminal("declaration-sequence");
    auto declaration_sequence_optional = NonTerminal("declaration-sequence-optional");

    auto translation_unit = NonTerminal("translation-unit");


    auto terminal_identifier = Terminal("ID");
    auto terminal_literal = Terminal("LITERAL");
    std::vector<GrammarSymbol> terminal_assignment_ops = {
        Terminal("="), Terminal("+="), Terminal("-="), Terminal("*="),
        Terminal("/="), Terminal("%="), Terminal(">>="), Terminal("<<="),
        Terminal("&="), Terminal("|="), Terminal("^=")
    };
    std::vector<GrammarSymbol> terminal_unary_ops = {
        Terminal("!"), Terminal("+"), Terminal("-"), Terminal("*"), Terminal("&"), Terminal("~")
    };
    auto terminal_logical_or_op = Terminal("||");
    auto terminal_logical_and_op = Terminal("&&");
    auto terminal_inclusive_or_op = Terminal("|");
    auto terminal_exclusive_or_op = Terminal("^");
    auto terminal_bitwise_and_op = Terminal("&");
    auto terminal_equality_op1 = Terminal("==");
    auto terminal_equality_op2 = Terminal("!=");
    auto terminal_shift_op1 = Terminal("<<");
    auto terminal_shift_op2 = Terminal(">>");
    auto terminal_relation_op1 = Terminal("<");
    auto terminal_relation_op2 = Terminal(">");
    auto terminal_relation_op3 = Terminal(">=");
    auto terminal_relation_op4 = Terminal("<=");
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
    auto terminal_comma = Terminal(",");
    std::vector<GrammarSymbol> terminal_types = {
        Terminal("int"), Terminal("float"), Terminal("double"), Terminal("char"),
        Terminal("bool"), Terminal("long"), Terminal("short"), Terminal("void"), Terminal("auto")
    };
    auto terminal_return = Terminal("return");

    auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

    std::unique_ptr<Grammar> p_grammar(new Grammar(translation_unit));

    (*p_grammar)
        (translation_unit, { { declaration_sequence } })
        (declaration_sequence_optional, { { declaration_sequence }, { lambda_symbol } })
        (declaration_sequence, { { declaration, declaration_sequence }, { declaration } })
        (declaration, { { function_definition }, { variable_definition } })

        (variable_definition, { { type_specifier, identifier_list, terminal_semicolon } })
        (identifier_list, { { terminal_identifier, terminal_comma, identifier_list }, {terminal_identifier} })

        (function_definition, { {function_declarator, function_body} })
        (function_declarator, { {type_specifier, terminal_identifier, terminal_left_paren, parameter_declaration_list_optional, terminal_right_paren} })
        (function_body, { {compound_statement} })

        (parameter_declaration_list_optional, { { parameter_declaration_list }, { lambda_symbol } })
        (parameter_declaration_list, { { parameter_declaration, terminal_comma, parameter_declaration_list }, { parameter_declaration }})

        (parameter_declaration, { { type_specifier, terminal_identifier } })
        (type_specifier, { { terminal_identifier }, { terminal_types[0] }, { terminal_types[1] }, { terminal_types[2] }, { terminal_types[3] }, { terminal_types[4] }, { terminal_types[5] }, {terminal_types[6]} })

        (statement, { { expression_statement }, { if_statement }, { while_statement }, { return_statement }, {variable_definition}, { compound_statement } })

        (compound_statement, { { terminal_left_brace, statement_sequence_optional, terminal_right_brace } })

        (statement_sequence_optional, { { statement_sequence }, { lambda_symbol } })
        (statement_sequence, { { statement, statement_sequence }, { statement } })

        (expression_statement, { { expression_optional, terminal_semicolon } })
        (expression_optional, { { expression }, { lambda_symbol } })

        (if_statement, { { terminal_if, terminal_left_paren, expression, terminal_right_paren, statement, terminal_else, statement }, { terminal_if, terminal_left_paren, expression, terminal_right_paren, statement } })

        (while_statement, { { terminal_while, terminal_left_paren, expression, terminal_right_paren, statement } })

        (return_statement, { { terminal_return, expression, terminal_semicolon }, {terminal_return, terminal_semicolon} })

        (primary_expression, { { terminal_identifier, terminal_left_paren, expression_list_optional, terminal_right_paren }, { terminal_left_paren, expression, terminal_right_paren }, { terminal_identifier },
        { terminal_literal }})

        (unary_expression, { {unary_operator, primary_expression}, { primary_expression } })

        (unary_operator, { { terminal_unary_ops[0] }, { terminal_unary_ops[1] }, { terminal_unary_ops[2] }, { terminal_unary_ops[3] }, { terminal_unary_ops[4] }, {terminal_unary_ops[5]} })

        (multiplicative_expression, { { unary_expression, multiplicative_expression_ex } })
        (multiplicative_expression_ex, { { terminal_mult_op1, unary_expression, multiplicative_expression_ex }, { terminal_mult_op2, unary_expression, multiplicative_expression_ex }, { terminal_mult_op3, unary_expression, multiplicative_expression_ex }, { lambda_symbol } })

        (additive_expression, { { multiplicative_expression, additive_expression_ex } })
        (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, { terminal_add_op2, terminal_identifier, additive_expression_ex }, { lambda_symbol } })

        (shift_expression, { { additive_expression, shift_expression_ex } })
        (shift_expression_ex, { { terminal_shift_op1, additive_expression, shift_expression_ex }, { terminal_shift_op2, additive_expression, shift_expression_ex }, {lambda_symbol} })

        (relational_expression, { {shift_expression, relational_expression_ex} })
        (relational_expression_ex, { {terminal_relation_op1, shift_expression, relational_expression_ex}, {terminal_relation_op2, shift_expression, relational_expression_ex}, {terminal_relation_op3, shift_expression, relational_expression_ex}, {terminal_relation_op4, shift_expression, relational_expression_ex}, {lambda_symbol} })

        (equality_expression, { { relational_expression, equality_expression_ex } })
        (equality_expression_ex, { { terminal_equality_op1, relational_expression , equality_expression_ex }, { terminal_equality_op2, relational_expression, equality_expression_ex }, { lambda_symbol } })

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
        (assingment_operator, { 
            { terminal_assignment_ops[0] }, { terminal_assignment_ops[1] }, { terminal_assignment_ops[2] },
            { terminal_assignment_ops[3] }, { terminal_assignment_ops[4] }, { terminal_assignment_ops[5] },
            { terminal_assignment_ops[6] }, { terminal_assignment_ops[7] }, { terminal_assignment_ops[8] },
            { terminal_assignment_ops[9] }, { terminal_assignment_ops[10] }, })
        (expression, { { assingment_expression } })
        (expression_list, { { expression, terminal_comma, expression_list }, {expression} })
        (expression_list_optional, { { expression_list }, {lambda_symbol} })
        ;

    return std::move(p_grammar);
}

