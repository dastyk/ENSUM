#include "Ensum_components\DataManager.h"


namespace Ensum
{
	namespace Components
	{
		DataManager::DataManager(EntityManager & entManager) : Manager(nullptr, entManager)
		{
			DataBuffer buff;

			buff.AddBoolValue("hej", true);
			buff.AddStringValue("hej2", "Teststring");

			string s = buff.GetStringValue("hej2");

			buff.SetStringValue("hej2", "StringChanged");
			
			s = buff.GetStringValue("hej2");
		}

		DataManager::~DataManager()
		{
		}
		const void DataManager::_Allocate(uint32_t size)
		{
			return void();
		}
		const void DataManager::_Destroy(uint32_t index)
		{
			return void();
		}
	}
}