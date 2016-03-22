#ifndef _INI_H_
#define _INI_H_

#pragma once
#include <fstream>
#include <vector>
#include "ensumstring.h"
#include "dll_export.h"

namespace Ensum
{
	namespace FileHandler
	{
		/** A wrapper class for ini files.
		* This class stores the values in given ini file and saves any new values on delete.
		*/
		class ENSUM_FILEHANDLER_EXPORT ini
		{
		private:
			struct Key
			{
				string name;
				string value;
				Key(const string& sname) : name(sname)
				{
				}
				Key(const string& sname, const string& svalue) : name(sname), value(svalue)
				{
				}
			};
			struct Section
			{
				string name;
				std::vector<Key> keys;

				Section(const string& sname) : name(sname)
				{
				}

			};
		public:
			ini(const string& path);
			~ini();
			/** Looks up the value of the given name and section and returns it as a string.
			* If the sectino/name was not found, the default_value will be returned.
			*/
			string Get(const string& section, const string& name, const string& default_value)const;
			/** Looks up the value of the given name and section and returns it as a integer.
			* If the sectino/name was not found, the default_value will be returned.
			*/
			long GetInteger(const string& section, const string& name, long default_value)const;
			/** Looks up the value of the given name and section and returns it as a real.
			* If the sectino/name was not found, the default_value will be returned.
			*/
			double GetReal(const string& section, const string& name, double default_value)const;
			/** Looks up the value of the given name and section and returns it as a boolean.
			* If the sectino/name was not found, the default_value will be returned.
			*/
			bool GetBoolean(const string& section, const string& name, bool default_value)const;


			/** Set the string value of given name in given section.
			* If the sectino/name was not found they are added.
			*/
			const void Set(const string& section, const string& name, const string& value);
			/** Set the integer value of given name in given section.
			* If the sectino/name was not found they are added.
			*/
			const void SetInteger(const string& section, const string& name, long value);
			/** Set the real value of given name in given section.
			* If the sectino/name was not found they are added.
			*/
			const void SetReal(const string& section, const string& name, double value);
			/** Set the boolean value of given name in given section.
			* If the sectino/name was not found they are added.
			*/
			const void SetBoolean(const string& section, const string& name, bool value);

		private:
			/** Reads the ini file to buffer.
			* You can't really call this parsing, but hey it works.
			*/
			const void _ParseData(std::ifstream& file);
			/** Writes the buffer to file.
			* 
			*/
			const void _WriteData(std::ofstream& file);
			std::vector<Section>* _sections;
			string _path;
		};
	}
}
#endif