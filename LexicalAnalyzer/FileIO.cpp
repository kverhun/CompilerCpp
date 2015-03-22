#include "FileIO.h"
#include <fstream>
#include <sstream>

using namespace LexicalAnalysis;

//------------------------------------------------------------------------------
std::string LexicalAnalysis::FileIO::GetFileString(const std::string& i_filename)
{
	std::ifstream t(i_filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	auto str = buffer.str();
    return str;
}
