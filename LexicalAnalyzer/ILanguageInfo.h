#pragma once

#include <string>
#include <vector>
#include <map>

namespace LexicalAnalysis
{
	class DFA;

	class ILanguageInfo
	{
	public:
		typedef size_t TLexemeClass;
		typedef std::map<TLexemeClass, std::string> TLexemeClasses;

		typedef std::vector<DFA> TAutomataPool;
		typedef std::vector<TLexemeClass> TLexemeClassPriority;
		
		typedef std::map<TLexemeClass, TAutomataPool> TLexemeAutomataPoolMap;

	public:
		virtual ~ILanguageInfo() = 0 {}

		virtual const TLexemeAutomataPoolMap& GetLexemeAutomataPoolMap() const = 0;
		virtual const TLexemeClassPriority&   GetLexemeClassPriority() const = 0;
		virtual const TLexemeClasses&		  GetLexemeClasses() const = 0;

	};
}