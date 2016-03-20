#pragma once
#include <string>
#include <Windows.h>
#include <exception>

#ifndef Exception
#define Exception(...) throw Ensum::Utils::Exce(__VA_ARGS__)
namespace Ensum
{
	namespace Utils
	{
		/** Class used when throwing exceptions.
		*
		*/
		class Exce
		{
		public:
			Exce(const std::exception& e) : _msg(e.what()), _cap("Standard Exception") {}
			Exce(const std::string& msg = "", const std::string& cap = "Error") : _msg(msg), _cap(cap) {}

			/** Displayes a promt window of the exception.
			*
			*/
			const void Print()const { MessageBoxA(0, _msg.c_str(), _cap.c_str(), 0); }
		private:
			std::string _msg;
			std::string _cap;
		};
	}
}
#endif