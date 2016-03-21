#ifndef _INI_H_
#define _INI_H_

#pragma once
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "dll_export.h"

namespace Ensum
{
	namespace FileHandler
	{


		class ENSUM_FILEHANDLER_EXPORT string
		{
		public:
			string() : _string(nullptr) { _string = new std::string; }
			string(const char* str) : string() { (*_string) = str; }
			string(const string& other) : string() { (*(this->_string)) = (*(other._string)); }

			string(const long& other) : string() { (*_string) = std::to_string(other); }
			string(const double& other) : string() { (*_string) = std::to_string(other); }
			string(const bool& other) : string() { (*_string) = other ? "true" : "false"; }

			~string() { delete _string; }

			//string& operator+(const string& str) { *this + str; return *this; }
			string operator+(const char* str)const { return string(((*(this->_string)) + str).c_str()); }
			operator const char*() { return this->c_str(); }

			string& operator=(const char* str) { (*(this->_string)) = str; return *this; }
			string& operator=(const string& other) { (*(this->_string)) = (*(other._string)); return *this; }
			
			const bool operator==(const char* str) { return (*(this->_string)) == str; }
			const bool operator==(const string& other) { return (*(this->_string)) == (*(other._string)); }

			const char* c_str() { return _string->c_str(); }
			const void transform(){ std::transform((*_string).begin(), (*_string).end(), (*_string).begin(), ::tolower); }
			const void clear() { _string->clear(); }
			const void push_back(const char str) { _string->push_back(str); }
			const uint32_t size() { return static_cast<uint32_t>(_string->size()); }
		private:
			std::string* _string;
		};

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

			string Get(const string& section, const string& name, const string& default_value)const;

			long GetInteger(const string& section, const string& name, long default_value)const;

			double GetReal(const string& section, const string& name, double default_value)const;

			bool GetBoolean(const string& section, const string& name, bool default_value)const;



			const void Set(const string& section, const string& name, const string& value);

			const void SetInteger(const string& section, const string& name, long value);

			const void SetReal(const string& section, const string& name, double value);

			const void SetBoolean(const string& section, const string& name, bool value);

		private:

			const void _ParseData(std::ifstream& file);
			const void _WriteData(std::ofstream& file);
			std::vector<Section>* _sections;
			string _path;
			int _datasize;
		};
	}
}
#endif