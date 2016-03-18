#pragma once
#include <string>
#include <Windows.h>
#ifndef Exception
#define Exception(...) throw Ensum::Utils::Exce(__VA_ARGS__)
namespace Ensum
{
	namespace Utils
	{
		class Exce
		{
		public:
			Exce(const std::string& msg = "", const std::string& cap = "Error") : _msg(msg), _cap(cap) {}

			const void Print()const { MessageBoxA(0, _msg.c_str(), _cap.c_str(), 0); }
		private:
			std::string _msg;
			std::string _cap;
		};
	}
}
#endif