#pragma once

#include <string>

namespace LexicalAnalysis
{
	class FileIO
	{
    public:
		static std::string GetFileString(const std::string& i_filename);
	};
}