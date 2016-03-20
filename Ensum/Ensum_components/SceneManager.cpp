#include "Ensum_components\SceneManager.h"
#include "Ensum_utils\Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Safe_Delete.h"


namespace Ensum
{
	namespace Components
	{
		SceneManager::SceneManager() :_entityToIndex(nullptr)
		{
			_entityToIndex = new std::unordered_map<Entity, uint32_t, EntityHasher>;
			_Allocate(10);
		}


		SceneManager::~SceneManager()
		{
			for (auto& scene : (*_entityToIndex))
			{
				SAFE_DELETE(_sceneData.scenePtr[scene.second]);
			}
			SAFE_DELETE(_entityToIndex);

			std::free(_sceneData.meta.buffer);
		}

		//template<typename SceneType>
		//inline const Entity & SceneManager::CreateScene(Input::Input * input)
		//{
		//	Scene * scene = new SceneType(_entityManager, input);

		//	if (_sceneData.meta.used >= _sceneData.meta.allocated)
		//		_Allocate(static_cast<uint32_t>(_sceneData.meta.allocated * 1.5f) + 10);

		//	const Entity& ent = scene->GetEntity();
		//	Utils::ConsoleLog::DumpToConsole("Creating Scene with id: %d", ent.ID);

		//	uint32_t index = (*_entityToIndex)[ent];
		//	_sceneData.entity[index] = ent;
		//	_sceneData.scenePtr[index] = scene;
		//	_sceneData.sceneUpdate[index] = true;

		//	return ent;
		//}
		const Entity& SceneManager::CreateScene(Scene * scene)
		{
			if (!scene) Exception("Scene was null.");

			if (_sceneData.meta.used >= _sceneData.meta.allocated)
				_Allocate(static_cast<uint32_t>(_sceneData.meta.allocated * 1.5f) + 10);

			const Entity& ent = scene->GetEntity();
			Utils::ConsoleLog::DumpToConsole("Creating Scene with id: %d", ent.ID);
			
			uint32_t index = (*_entityToIndex)[ent] = _entityToIndex->size();
			_sceneData.entity[index] = ent;
			_sceneData.scenePtr[index] = scene;
			_sceneData.sceneUpdate[index] = true;
			_sceneData.meta.used++;
			return ent;
		}
		const void SceneManager::RemoveScene(const Entity& sceneEntity)
		{
			auto& find = _entityToIndex->find(sceneEntity);
			if (find != _entityToIndex->end())
			{
				_Destroy(find->second);
			}
			return void();
		}
		const void SceneManager::SetUpdate(const Entity& sceneEntity, bool update)
		{
			auto& find = _entityToIndex->find(sceneEntity);
			if (find != _entityToIndex->end())
			{
				_sceneData.sceneUpdate[find->second] = update;
			}
		}
		const void SceneManager::Frame()
		{
			_GC();

			for (uint32_t i = 0; i < _sceneData.meta.used; i++)
			{
				if (_sceneData.sceneUpdate[i]) _sceneData.scenePtr[i]->Frame();
			}
			return void();
		}
		EntityManager& SceneManager::GetEntityManager()
		{
			return _entityManager;
		}
		const void SceneManager::_Allocate(uint32_t size)
		{
			if (size <= _sceneData.meta.allocated) Exception("Alloc should only increase.");

			SceneData new_data;
			const unsigned bytes = size * (sizeof(Entity) + sizeof(Scene*) + sizeof(bool));
			MetaData& mdata = new_data.meta;
			mdata.buffer = std::malloc(bytes);
			mdata.used = _sceneData.meta.used;
			mdata.allocated = size;

			new_data.entity = (Entity*)(mdata.buffer);
			new_data.scenePtr = (Scene**)(new_data.entity + size);
			new_data.sceneUpdate = (bool *)(new_data.scenePtr + size);

			memcpy(new_data.entity, _sceneData.entity, _sceneData.meta.used * sizeof(Entity));
			memcpy(new_data.scenePtr, _sceneData.scenePtr, _sceneData.meta.used * sizeof(Scene*));
			memcpy(new_data.sceneUpdate, _sceneData.sceneUpdate, _sceneData.meta.used * sizeof(bool));


			std::free(_sceneData.meta.buffer);
			_sceneData = new_data;
		}
		const void SceneManager::_Destroy(uint32_t index)
		{
			unsigned last = _sceneData.meta.used - 1;
			const Entity& e = _sceneData.entity[index];
			const Entity& last_e = _sceneData.entity[last];

			Utils::ConsoleLog::DumpToConsole("Deleting Scene with id: %d", e.ID);

			_sceneData.entity[index] = _sceneData.entity[last];
			SAFE_DELETE(_sceneData.scenePtr[index]);
			_sceneData.scenePtr[index] = _sceneData.scenePtr[last];
			_sceneData.sceneUpdate[index] = _sceneData.sceneUpdate[last];

			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_sceneData.meta.used--;
		}
		const void SceneManager::_GC()
		{
			uint32_t alive_in_row = 0;
			while (_sceneData.meta.used > 0 && alive_in_row < 4U) 
			{
				std::uniform_int_distribution<uint32_t> distribution(0U, _sceneData.meta.used - 1U);
				uint32_t i = distribution(_generator);
				if (_entityManager.Alive(_sceneData.entity[i])) 
				{
					alive_in_row++;
					continue;
				}
				alive_in_row = 0;
				_Destroy(i);
			}
		}
	}
}