#include "SymbolTable.h"

using namespace SyntaxAnalysis;

//------------------------------------------------------------------------------
SymbolTable::SymbolTable(size_t i_idx, size_t i_parent_idx)
: m_index(i_idx)
, m_parent_index(i_parent_idx)
{

}

//------------------------------------------------------------------------------
bool SymbolTable::AddVariable(const std::string& i_name, const std::string& i_type)
{
    if (m_variables.find(i_name) != m_variables.end() ||
        m_functions.find(i_name) != m_functions.end())
        return false;

    m_variables[i_name] = i_type;
    return true;
}


//------------------------------------------------------------------------------
bool SymbolTable::AddFunction(const std::string& i_name, const std::string& i_return_type, const std::vector<std::pair<std::string, std::string>>& i_params)
{
    if (m_variables.find(i_name) != m_variables.end() ||
        m_functions.find(i_name) != m_functions.end())
        return false;
    
    m_functions[i_name] = std::make_pair(i_return_type, i_params);
    return true;
}

//------------------------------------------------------------------------------
std::string SyntaxAnalysis::SymbolTable::ToString() const
{
    std::string res;

    res.append("TABLE #" + std::to_string(m_index) + "; Parent: " + std::to_string(m_parent_index) + "\n");

    for (const auto& f : m_functions)
    {
        res.append("\tFUNC: " + f.first + "; RET TYPE: " + f.second.first + "; PARAMS: ");
        if (f.second.second.empty())
            res.append("no params");
        for (auto p : f.second.second)
            res.append("[NAME: " + p.first + ", TYPE: " + p.second + "]; ");
        res.append("\n");
    }

    for (const auto& v : m_variables)
    {
        res.append("\tVAR: " + v.first + ", TYPE: " + v.second + "\n");
    }

    return res;
}


//------------------------------------------------------------------------------
size_t SymbolTable::GetIndex() const
{
    return m_index;
}

//------------------------------------------------------------------------------
size_t SymbolTable::GetParentIndex() const
{
    return m_parent_index;
}
