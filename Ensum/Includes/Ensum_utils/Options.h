#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#pragma once
#include "dll_export.h"
#include "Event.h"
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
		/** A singleton class for reading and writing to config file.
		*
		*/
		class Options
		{
		private:
			Options();
			~Options();
			static Options* _instance;

			/** Private Read function.
			* 
			*/
			long _GetIntegerOption(const string& section, const string& option, long default_value);
			/** Private Read function.
			*
			*/
			double _GetRealOption(const string& section, const string& option, double default_value);
			/** Private Read function.
			*
			*/
			bool _GetBooleanOption(const string& section, const string& option, bool default_value);
			/** Private Read function.
			*
			*/
			string _GetStringOption(const string& section, const string& option, const string& default_value);

			/** Private write function.
			*
			*/
			void _SetIntegerOption(const string& section, const string& option, long value);
			/** Private write function.
			*
			*/
			void _SetRealOption(const string& section, const string& option, double value);
			/** Private write function.
			*
			*/
			void _SetBooleanOption(const string& section, const string& option, bool value);
			/** Private write function.
			*
			*/
			void _SetStringOption(const string& section, const string& option, const string& value);
		public:	
		

			/** Create the options instance.
			* If options are not used all clients will simply use default values.
			*/
			ENSUM_UTILS_EXPORT static void CreateInstance();
			/** Deletes the instance.
			*
			*/
			ENSUM_UTILS_EXPORT static void DeleteInstance();

			/** Public read function.
			*
			*/
			ENSUM_UTILS_EXPORT static long GetIntegerOption(const string& section, const string& option, long default_value);
			/** Public read function.
			*
			*/
			ENSUM_UTILS_EXPORT static double GetRealOption(const string& section, const string& option, double default_value);
			/** Public read function.
			*
			*/
			ENSUM_UTILS_EXPORT static bool GetBooleanOption(const string& section, const string& option, bool default_value);
			/** Public read function.
			*
			*/
			ENSUM_UTILS_EXPORT static string GetStringOption(const string& section, const string& option, const string& default_value);

			/** Public write function.
			*
			*/
			ENSUM_UTILS_EXPORT static void SetIntegerOption(const string& section, const string& option, long value);
			/** Public write function.
			*
			*/
			ENSUM_UTILS_EXPORT static void SetRealOption(const string& section, const string& option, double value);
			/** Public write function.
			*
			*/
			ENSUM_UTILS_EXPORT static void SetBooleanOption(const string& section, const string& option, bool value);
			/** Public write function.
			*
			*/
			ENSUM_UTILS_EXPORT static void SetStringOption(const string& section, const string& option, const string& value);
			
			/** Subscribe to the OptionsChange event.
			* When a change occurs in options this event is called.
			*/
			ENSUM_UTILS_EXPORT static void Subscribe(Delegate<void()>& dele);
			/** UnSubscribe to the OptionsChange event.
			* 
			*/
			ENSUM_UTILS_EXPORT static void UnSubscribe(Delegate<void()>& dele);
		private:
			FileHandler::ini* _file;
			Event<void()> _OptionChange;

		};
	}
}
#endif