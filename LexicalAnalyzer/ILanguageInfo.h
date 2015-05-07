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
		typedef std::vector<char> TAlphabet;
		
		typedef size_t TLexemeClass;
		typedef std::map<TLexemeClass, std::string> TLexemeClasses;

		typedef std::vector<DFA> TAutomataPool;
		typedef std::vector<TLexemeClass> TLexemeClassPriority;
		
		typedef std::map<TLexemeClass, TAutomataPool> TLexemeAutomataPoolMap;

        enum {SEPARATOR_LEXEME = 1023, ERROR_LEXEME = 1024};

	public:
		virtual ~ILanguageInfo() = 0 {}

		virtual const TAlphabet&			  GetAlphabet() const = 0;
		virtual const TLexemeAutomataPoolMap& GetLexemeAutomataPoolMap() const = 0;
		virtual const TLexemeClassPriority&   GetLexemeClassPriority() const = 0;
		virtual const TLexemeClasses&		  GetLexemeClasses() const = 0;

        virtual bool IsCharSeparator(char i_char) const = 0;
	};
}