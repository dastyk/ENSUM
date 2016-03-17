#pragma once

//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)

#ifdef ENSUM_CORE_EXPORTS
#	define ENSUM_CORE_EXPORT __declspec(dllexport)
#	define ENSUM_CORE_TEMPLATE
#else
#	define ENSUM_CORE_EXPORT __declspec(dllimport)
#	define ENSUM_CORE_TEMPLATE extern
#endif // ENSUM_UTILS_EXPORTS
