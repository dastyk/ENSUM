#include "Ensum_utils\Options.h"
#include "Safe_Delete.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Exception.h"

namespace Ensum
{
	namespace Utils
	{
		Options* Options::_instance = nullptr;


		Options::Options()
		{
			ConsoleLog::DumpToConsole("Creating options instance");
			try{ _file = new FileHandler::ini("config.ini"); }
			catch(const Utils::Exce& e)
			{
				e;
				std::ofstream file;
				file.open("config.ini", std::ios::out | std::ios::trunc);
				file.close();
				_file = new FileHandler::ini("config.ini");
			}
		}
		Options::~Options()
		{
			ConsoleLog::DumpToConsole("Deleting options instance");
			SAFE_DELETE(_file);
		}
		long Options::_GetIntegerOption(const string & section, const string & option, long default_value)
		{
			return _file->GetInteger(section, option, default_value);
		}

		double Options::_GetRealOption(const string & section, const string & option, double default_value)
		{
			return _file->GetReal(section, option, default_value);
		}

		bool Options::_GetBooleanOption(const string & section, const string & option, bool default_value)
		{
			return _file->GetBoolean(section, option, default_value);
		}

		string Options::_GetStringOption(const string & section, const string & option, const string & default_value)
		{
			return _file->Get(section, option, default_value);
		}
		void Options::_SetIntegerOption(const string & section, const string & option, long value)
		{
			_file->SetInteger(section, option, value);
		}
		void Options::_SetRealOption(const string & section, const string & option, double value)
		{
			_file->SetReal(section, option, value);
		}
		void Options::_SetBooleanOption(const string & section, const string & option, bool value)
		{
			_file->SetReal(section, option, value);
		}
		void Options::_SetStringOption(const string & section, const string & option, const string & value)
		{
			_file->Set(section, option, value);
		}
		void Options::CreateInstance()
		{
			try {
				if (!_instance)
					_instance = new Options;
			}
			catch (const Utils::Exce& e) { e.Print(); SAFE_DELETE(_instance); }
		}
		void Options::DeleteInstance()
		{
			if (_instance)
				SAFE_DELETE(_instance);
		}
		long Options::GetIntegerOption(const string & section, const string & option, long default_value)
		{
			if (!_instance)
				return default_value;

			return _instance->_GetIntegerOption(section, option, default_value);
		}
		double Options::GetRealOption(const string & section, const string & option, double default_value)
		{
			if (!_instance)
				return default_value;

			return _instance->_GetRealOption(section, option, default_value);
		}
		bool Options::GetBooleanOption(const string & section, const string & option, bool default_value)
		{
			if (!_instance)
				return default_value;

			return _instance->_GetBooleanOption(section, option, default_value);
		}
		string Options::GetStringOption(const string & section, const string & option, const string & default_value)
		{
			if (!_instance)
				return default_value;

			return _instance->_GetStringOption(section, option, default_value);
		}

		void Options::SetIntegerOption(const string & section, const string & option, long value)
		{
			if (!_instance)
				return;
			return _instance->_SetIntegerOption(section, option, value);
		}

		void Options::SetRealOption(const string & section, const string & option, double value)
		{
			if (!_instance)
				return;
			return _instance->_SetRealOption(section, option, value);
		}
		void Options::SetBooleanOption(const string & section, const string & option, bool value)
		{
			if (!_instance)
				return;
			return _instance->_SetBooleanOption(section, option, value);
		}
		void Options::SetStringOption(const string & section, const string & option, const string & value)
		{
			if (!_instance)
				return;
			return _instance->_SetStringOption(section, option, value);
		}
	}
}