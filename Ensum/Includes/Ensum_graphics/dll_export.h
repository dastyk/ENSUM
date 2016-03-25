#pragma once

//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)
//disable warnings on member not being exported
//#pragma warning (disable : 4251)

#ifdef ENSUM_GRAPHICS_EXPORTS
#	define ENSUM_GRAPHICS_EXPORT __declspec(dllexport)
#	define ENSUM_GRAPHICS_TEMPLATE
#else
#	define ENSUM_GRAPHICS_EXPORT __declspec(dllimport)
#	define ENSUM_GRAPHICS_TEMPLATE extern
#endif // ENSUM_UTILS_EXPORTS
