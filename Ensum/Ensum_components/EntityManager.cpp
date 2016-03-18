#include "Ensum_components\EntityManager.h"

#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Exception.h"
#include "Ensum_utils\Safe_Delete.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif

namespace Ensum
{
	namespace Components
	{

		EntityManager::EntityManager() :_freeIndices(nullptr), _generation(nullptr)
		{
			_freeIndices = new std::deque<uint32_t>;
			if (!_freeIndices) Exception("Could not create freeIndices que.");

			_generation = new std::vector<uint8_t>;
			if (!_generation) Exception("Could not create generation vector.");
		}


		EntityManager::~EntityManager()
		{
			SAFE_DELETE(_freeIndices);
			SAFE_DELETE(_generation);
		}
		Entity EntityManager::Create()
		{
			uint32_t index = -1;
			if (_freeIndices->size() > MINIMUM_FREE_INDICES)
			{
				index = _freeIndices->front();
				_freeIndices->pop_front();
			}
			else
			{
				index = static_cast<uint32_t>(_generation->size());
			
				if (index >= ENTITY_INDEX_MASK)
				{
					_generation->clear();
					index = 0;
					Utils::ConsoleLog::DumpToConsole("To large entity index!.");
				}

				_generation->push_back(0);
			}
			return (index | (*_generation)[index] << ENTITY_INDEX_BITS);
		}
		const bool EntityManager::Alive(const Entity & entity) const
		{
			return entity.Generation() == (*_generation)[entity.Index()];
		}
		const void EntityManager::Delete(const Entity & entity)
		{
			const uint32_t idx = entity.Index();
			(*_generation)[idx]++;
			_freeIndices->push_back(idx);
		}
	}
}