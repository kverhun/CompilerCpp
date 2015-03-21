#pragma once

#ifdef LEXICALANALYZER_DLL
#define LEXICALANALYZER_API _declspec(dllexport)
#else
#define LEXICALANALYZER_API _declspec(dllimport)
#endif