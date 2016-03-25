#pragma once
#include <string>
#include <Windows.h>
#include <exception>
#include "ensumstring.h"

#ifdef ENSUM_EXPORTS
#	define ENSUM_EXPORT __declspec(dllexport)
#	define ENSUM_TEMPLATE
#else
#	define ENSUM_EXPORT __declspec(dllimport)
#	define ENSUM_TEMPLATE extern
#endif

//disable warnings on unsafe function
#pragma warning (disable : 4996)

#ifndef Exception
#define Exception(msg, ...) throw Ensum::Exce(msg, __VA_ARGS__)
namespace Ensum
{

	/** Class used when throwing exceptions.
	*
	*/
	class ENSUM_EXPORT Exce
	{
	public:
		Exce(const std::exception& e) : _msg(e.what()), _cap("Standard Exception") {}
		Exce(const char* msg, ...) : _cap("Error")
		{
			va_list args;
			va_start(args, msg);
			int len = vsnprintf(NULL, 0, msg, args) + 1;
			// instance found, add message to que.
			char* buffer = new char[len];

			vsprintf(buffer, msg, args);

			_msg = buffer;

			delete[] buffer;
			va_end(args);
		}

		/** Displayes a promt window of the exception.
		*
		*/
		const void Print()const { MessageBoxA(0, _msg.c_str(), _cap.c_str(), 0); }
	private:
		string _msg;
		string _cap;
	};

}
#endif