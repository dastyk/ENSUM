#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#pragma once
#include "dll_export.h"
#include "Ensum_filehandler\Ini.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_filehandlerD.lib")
#else
#pragma comment(lib, "Ensum_filehandler.lib")
#endif
namespace Ensum
{
	namespace Utils
	{
		class Options
		{
		private:
			Options();
			~Options();
			static Options* _instance;

			long _GetIntegerOption(const string& section, const string& option, long default_value);
			double _GetRealOption(const string& section, const string& option, double default_value);
			bool _GetBooleanOption(const string& section, const string& option, bool default_value);
			string _GetStringOption(const string& section, const string& option, const string& default_value);

			void _SetIntegerOption(const string& section, const string& option, long value);
			void _SetRealOption(const string& section, const string& option, double value);
			void _SetBooleanOption(const string& section, const string& option, bool value);
			void _SetStringOption(const string& section, const string& option, const string& value);
		public:	
		


			ENSUM_UTILS_EXPORT static void CreateInstance();
			ENSUM_UTILS_EXPORT static void DeleteInstance();

			ENSUM_UTILS_EXPORT static long GetIntegerOption(const string& section, const string& option, long default_value);
			ENSUM_UTILS_EXPORT static double GetRealOption(const string& section, const string& option, double default_value);
			ENSUM_UTILS_EXPORT static bool GetBooleanOption(const string& section, const string& option, bool default_value);
			ENSUM_UTILS_EXPORT static string GetStringOption(const string& section, const string& option, const string& default_value);

			ENSUM_UTILS_EXPORT static void SetIntegerOption(const string& section, const string& option, long value);
			ENSUM_UTILS_EXPORT static void SetRealOption(const string& section, const string& option, double value);
			ENSUM_UTILS_EXPORT static void SetBooleanOption(const string& section, const string& option, bool value);
			ENSUM_UTILS_EXPORT static void SetStringOption(const string& section, const string& option, const string& value);
		private:
			FileHandler::ini* _file;
		};
	}
}
#endif