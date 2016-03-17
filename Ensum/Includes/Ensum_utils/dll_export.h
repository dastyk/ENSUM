#pragma once

//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)

#ifdef ENSUM_UTILS_EXPORTS
#	define ENSUM_UTILS_EXPORT __declspec(dllexport)
#	define ENSUM_UTILS_TEMPLATE
#else
#	define ENSUM_UTILS_EXPORT __declspec(dllimport)
#	define ENSUM_UTILS_TEMPLATE extern
#endif // ENSUM_UTILS_EXPORTS
