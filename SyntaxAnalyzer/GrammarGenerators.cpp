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
    auto additive_expression = NonTerminal("additive-expression");
    auto additive_expression_ex = NonTerminal("additive-expression-ex");
    auto multiplicative_expression = NonTerminal("multiplicative-expression");
    auto multiplicative_expression_ex = NonTerminal("multiplicative-expression-ex");
    auto primary_expression = NonTerminal("primary-expression");
    auto expression = NonTerminal("expression");
    auto expression_optional = NonTerminal("expression-opt");
    auto expression_statement = NonTerminal("expression-statement");
    auto if_statement = NonTerminal("if-statement");
    auto while_statement = NonTerminal("while-statement");
    auto statement = NonTerminal("statement");
    auto statement_sequence = NonTerminal("statement-sequence");
    auto statement_sequence_optional = NonTerminal("statement-sequence-opt");
    auto compound_statement = NonTerminal("compound-statement");

    auto terminal = Terminal("ID");
    auto terminal_ass_op1 = Terminal("=");
    auto terminal_ass_op2 = Terminal("+=");
    auto terminal_logical_or_op = Terminal("||");
    auto terminal_logical_and_op = Terminal("&&");
    auto terminal_inclusive_or_op = Terminal("|");
    auto terminal_exclusive_or_op = Terminal("^");
    auto terminal_bitwise_and_op = Terminal("&");
    auto terminal_equality_op1 = Terminal("==");
    auto terminal_equality_op2 = Terminal("!=");
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

    auto lambda_symbol = GrammarSymbol(GrammarSymbol::GST_LAMBDA);

    std::unique_ptr<Grammar> p_grammar(new Grammar(statement));

    (*p_grammar)
        (statement, { { expression_statement }, { if_statement }, { while_statement }, { compound_statement } })

        (compound_statement, { { terminal_left_brace, statement_sequence_optional, terminal_right_brace } })

        (statement_sequence_optional, { { statement_sequence }, { lambda_symbol } })
        (statement_sequence, { { statement, statement_sequence }, { statement } })

        (expression_statement, { { expression_optional, terminal_semicolon } })
        (expression_optional, { { expression }, { lambda_symbol } })

        (if_statement, { { terminal_if, terminal_left_paren, expression, terminal_right_paren, statement, terminal_else, statement }, { terminal_if, terminal_left_paren, expression, terminal_right_paren, statement } })

        (while_statement, { { terminal_while, terminal_left_paren, expression, terminal_right_paren, statement } })

        (primary_expression, { { terminal }, { terminal_left_paren, expression, terminal_right_paren } })

        (multiplicative_expression, { { primary_expression, multiplicative_expression_ex } })
        (multiplicative_expression_ex, { { terminal_mult_op1, primary_expression, multiplicative_expression_ex }, { terminal_mult_op2, terminal, multiplicative_expression_ex }, { terminal_mult_op3, terminal, multiplicative_expression_ex }, { lambda_symbol } })

        (additive_expression, { { multiplicative_expression, additive_expression_ex } })
        (additive_expression_ex, { { terminal_add_op1, multiplicative_expression, additive_expression_ex }, { terminal_add_op2, terminal, additive_expression_ex }, { lambda_symbol } })

        (equality_expression, { { additive_expression, equality_expression_ex } })
        (equality_expression_ex, { { terminal_equality_op1, additive_expression, equality_expression_ex }, { terminal_ass_op2, terminal, equality_expression_ex }, { lambda_symbol } })

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
        (assingment_operator, { { terminal_ass_op1 }, { terminal_ass_op2 } })
        (expression, { { assingment_expression } })
        ;

    return std::move(p_grammar);
}

