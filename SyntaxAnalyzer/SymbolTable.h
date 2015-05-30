#pragma once

#include <string>
#include <map>
#include <vector>

namespace SyntaxAnalysis
{
    class SymbolTable
    {
    public:
        enum ESymbolType
        {
            ST_VARIABLE,
            ST_FUNCTION
        };

    public:
        SymbolTable(size_t i_idx, size_t i_parent_idx);

        bool AddVariable(const std::string& i_name, const std::string& i_type);
        bool AddFunction(const std::string& i_name, const std::string& i_return_type, const std::vector<std::pair<std::string, std::string>>& i_params);

        std::string ToString() const;

        size_t GetIndex() const;
        size_t GetParentIndex() const;

    private:
        std::map<std::string, std::string> m_variables; // name-to-type map
        std::map<std::string, std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> m_functions;
        size_t m_index;
        size_t m_parent_index;

    };

}