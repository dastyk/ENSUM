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

#ifndef Exception
#define Exception(...) throw Ensum::Exce(__VA_ARGS__)
namespace Ensum
{

	/** Class used when throwing exceptions.
	*
	*/
	class ENSUM_EXPORT Exce
	{
	public:
		Exce(const std::exception& e) : _msg(e.what()), _cap("Standard Exception") {}
		Exce(const string& msg = "", const string& cap = "Error") : _msg(msg), _cap(cap) {}

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