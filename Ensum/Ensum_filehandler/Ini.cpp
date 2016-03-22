#include "Ensum_filehandler\Ini.h"
#include "Safe_Delete.h"
#include "Ensum_utils\Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif
namespace Ensum
{
	namespace FileHandler
	{
		ini::ini(const string& path) : _sections(nullptr), _path(path)
		{
			_sections = new std::vector<Section>;

			std::ifstream file;

			file.open(_path, std::ios::in);
			if (file.is_open()) 
				_ParseData(file);
			

	
			file.close();

		}


		ini::~ini()
		{
			std::ofstream file;

			file.open(_path, std::ios::out | std::ios::trunc);
			if (file.is_open()) 
				_WriteData(file);

			file.close();

			SAFE_DELETE(_sections);
		}
		string ini::Get(const string & section, const string & name, const string & default_value) const
		{
			for (auto& s : *_sections)
			{
				if (s.name == section)
				{
					for (auto& key : s.keys)
					{
						if (key.name == name)
							return key.value;
					}
				}
			}
			return default_value;
		}
		long ini::GetInteger(const string & section, const string & name, long default_value) const
		{
			string valstr = Get(section, name, "");
			const char* value = valstr;
			char* end;
			// This parses "1234" (decimal) and also "0x4D2" (hex)
			long n = strtol(value, &end, 0);
			return end > value ? n : default_value;
		}
		double ini::GetReal(const string & section, const string & name, double default_value) const
		{
			string valstr = Get(section, name, "");
			const char* value = valstr;
			char* end;
			double n = strtod(value, &end);
			return end > value ? n : default_value;
		}
		bool ini::GetBoolean(const string & section, const string & name, bool default_value) const
		{
			string valstr = Get(section, name, "");
			// Convert to lower case to make string comparisons case-insensitive
			valstr.transform();
			if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
				return true;
			else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
				return false;
			else
				return default_value;
		}
		const void ini::Set(const string & section, const string & name, const string & value)
		{
			for (auto& s : *_sections)
			{
				if (s.name == section)
				{
					for (auto& key : s.keys)
					{
						if (key.name == name)
						{
							key.value = value;
							return;
						}
					}

					s.keys.push_back(Key(name, value));
					return;
				}
			}

			_sections->push_back(Section(section));
			std::vector<Key>& k = (*_sections)[_sections->size() - 1].keys;
			k.push_back(Key(name, value));
		}
		const void ini::SetInteger(const string & section, const string & name, long value)
		{
			Set(section, name, value);
			return void();
		}
		const void ini::SetReal(const string & section, const string & name, double value)
		{
			Set(section, name, value);
			return void();
		}
		const void ini::SetBoolean(const string & section, const string & name, bool value)
		{
			Set(section, name, value);
			return void();
		}
		const void ini::_ParseData(std::ifstream & file)
		{
			string name;

			char data;
			while (file.get(data))
			{
				if (data == '[')
				{
					name.clear();
					// Read the section name
					file.read(&data, 1);
					while (data != ']')
					{
						name.push_back(data);
						file.read(&data, 1);
						if (name.size() > 50) Exception("Failed to load ini file.");
					}
					_sections->push_back(Section(name));
					std::string temp;
					std::getline(file, temp);
				}
				else if (data != '\n')
				{
					name.clear();
					while (data != '=')
					{
						name.push_back(data);
						file.read(&data, 1);
						if (name.size() > 50) Exception("Failed to load ini file.");
					}
					std::vector<Key>& k = (*_sections)[_sections->size() - 1].keys;
					k.push_back(Key(name));
					name.clear();
					std::string temp;
					getline(file, temp);
					k.at(k.size() - 1).value = temp.c_str();
				}
			}
			return void();
		}
		const void ini::_WriteData(std::ofstream & file)
		{
			for (auto& section : *_sections)
			{
				file << (string("[") + section.name) + "]\n";
				for (auto& key : section.keys)
				{
					file << (key.name + "=" + key.value + "\n");
				}
				file << "\n";
			}
			return void();
		}
	}
}