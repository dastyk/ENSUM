#include "Ensum_components\EntityManager.h"

#include "Ensum_utils\ConsoleLog.h"
#include "Exception.h"
#include "Safe_Delete.h"

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

			_destroyCallback = new std::vector<Event<const void(const Entity& entity)>>;
			if (!_destroyCallback) Exception("Could not create destroyCallback vector.");
		}


		EntityManager::~EntityManager()
		{
			SAFE_DELETE(_destroyCallback);
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
					
					Utils::ConsoleLog::DumpToConsole("To large entity index! %d.", index );
					index = 0;
				}

				_generation->push_back(0);
			}
			return (index | (*_generation)[index] << ENTITY_INDEX_BITS);
		}
		const void EntityManager::AddDeleteCallback(const Entity & entity, Delegate<const void (const Entity& entity)>& callback)
		{
			if (Alive(entity))
			{
				const uint32_t idx = entity.Index();
				(*_destroyCallback)[idx]+=callback;
			}
		}
		const bool EntityManager::Alive(const Entity & entity) const
		{
			uint8_t t = entity.Generation();
			uint8_t t2 = (*_generation)[entity.Index()];
			return entity.Generation() == (*_generation)[entity.Index()];
		}
		const void EntityManager::Delete(const Entity & entity)
		{
			uint8_t t = entity.Generation();
			uint8_t t2 = (*_generation)[entity.Index()];
			if (entity.Generation() == (*_generation)[entity.Index()])
			{
				const uint32_t idx = entity.Index();
				(*_generation)[idx]++;
				if (idx < _destroyCallback->size())
				{
					(*_destroyCallback)[idx](entity);
					(*_destroyCallback)[idx].Clear();
				}
				_freeIndices->push_back(idx);
			}
		}
	}
}