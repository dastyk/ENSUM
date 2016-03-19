#ifndef _CONSOLE_LOG_H_
#define _CONSOLE_LOG_H_

#pragma once
#include <deque>
#include <thread>
#include <process.h>
#include <stdint.h>

#include "dll_export.h"
#include <Windows.h>

#ifndef WriteToLog
#define WriteToLog(message, ...) Ensum::Utils::ConsoleLog::DumpToConsole(message, __VA_ARGS__)
#endif

namespace Ensum
{
	namespace Utils
	{
		class ENSUM_UTILS_EXPORT ConsoleLog
		{
		private:
			ConsoleLog();
			static ConsoleLog* _instance;

			std::deque<std::string>* _toPrint;


			HANDLE _threadHandle;
			HANDLE _writeMutex;
			DWORD _threadAddress;

			bool _shutdown;

			//DWORD WINAPI ThreadEntryPoint(LPVOID lpParam);
		public:
			~ConsoleLog();

			const static void CreateInstance();
			const static void DeleteInstance();
			const static void DumpToConsole(const char* message, ...);

			const void AddToQue(const std::string& data);

			const void Write();

		};

	}
}
#endif