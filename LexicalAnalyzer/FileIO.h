#pragma once

#include <string>

namespace LexicalAnalyzer
{
	class FileIO
	{
		std::string GetFileString(const std::string& i_filename);
	};
}