#include "FileIO.h"
#include <fstream>
#include <sstream>

using namespace LexicalAnalyzer;

//------------------------------------------------------------------------------
std::string LexicalAnalyzer::FileIO::GetFileString(const std::string& i_filename)
{
	std::ifstream t(i_filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}
