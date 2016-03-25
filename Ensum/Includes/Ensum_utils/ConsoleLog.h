#ifndef _CONSOLE_LOG_H_
#define _CONSOLE_LOG_H_

#pragma once
#include <deque>
#include <thread>
#include <process.h>
#include <stdint.h>
#include "ensumstring.h"
#include "dll_export.h"
#include <Windows.h>

#ifndef WriteToLog
#define WriteToLog(message, ...) Ensum::Utils::ConsoleLog::DumpToConsole(message, __VA_ARGS__)
#endif

namespace Ensum
{
	namespace Utils
	{
		/** Debug console.
		*
		*/
		class ENSUM_UTILS_EXPORT ConsoleLog
		{
		private:
			ConsoleLog();
			static ConsoleLog* _instance;

			std::deque<string>* _toPrint;


			HANDLE _threadHandle;
			HANDLE _writeMutex;
			DWORD _threadAddress;

			bool _shutdown;
		public:
			~ConsoleLog();
			/** Creates the console.
			*
			*/
			const static void CreateInstance();
			/** Deletes the console.
			*
			*/
			const static void DeleteInstance();
			/** Adds the message and optional args to the que.
			* Use this static function when writing to the console.
			* This is a formated string. (See documentation for printf).
			*/
			const static void DumpToConsole(const char* message, ...);

			/** Adds the string data to the que.
			*
			*/
			const void AddToQue(const string& data);

			/** Writes the first items in the que to the console, then deletes it.
			* This is used in the thread.
			*/
			const void Write();

		};

	}
}
#endif