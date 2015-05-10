#include "Grammar.h"

using namespace SyntaxAnalysis;

Grammar::Grammar()
{

}

Grammar& Grammar::operator()(const TProductionLHS& i_lhs, const TProductionRHS& i_rhs)
{
    m_productions[i_lhs] = i_rhs;
    return *this;
}

const TProductionRHS& Grammar::GetProduction(const TProductionLHS& i_lhs) const
{
    auto it_res = m_productions.find(i_lhs);
    if (it_res == m_productions.end())
        throw std::logic_error("No such production");
        
    return it_res->second;
}
