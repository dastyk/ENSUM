#pragma once

//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)

#ifdef ENSUM_INPUT_EXPORTS
#	define ENSUM_INPUT_EXPORT __declspec(dllexport)
#	define ENSUM_INPUT_TEMPLATE
#else
#	define ENSUM_INPUT_EXPORT __declspec(dllimport)
#	define ENSUM_INPUT_TEMPLATE extern
#endif // ENSUM_INPUT_EXPORTS
