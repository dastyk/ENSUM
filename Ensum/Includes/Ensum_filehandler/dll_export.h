#pragma once

//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)

#ifdef ENSUM_FILEHANDLER_EXPORTS
#	define ENSUM_FILEHANDLER_EXPORT __declspec(dllexport)
#	define ENSUM_FILEHANDLER_TEMPLATE
#else
#	define ENSUM_FILEHANDLER_EXPORT __declspec(dllimport)
#	define ENSUM_FILEHANDLER_TEMPLATE extern
#endif // ENSUM_FILEHANDLER_EXPORTS
