#pragma once

#include <memory>

namespace SyntaxAnalysis
{
    class Grammar;

    std::unique_ptr<Grammar> GenerateGrammarCpp();
}