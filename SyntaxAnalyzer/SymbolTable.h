#pragma once

#include <string>
#include <map>

namespace SyntaxAnalysis
{
    class SymbolTable
    {
    public:
        SymbolTable(size_t i_idx, size_t i_parent_idx);

        bool AddVariable(const std::string& i_name, const std::string& i_type);

        std::string ToString() const;

        size_t GetIndex() const;
        size_t GetParentIndex() const;

    private:
        std::map<std::string, std::string> m_variables; // name-to-type map
        size_t m_index;
        size_t m_parent_index;

    };

}