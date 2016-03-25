#include "Ensum_components\SceneManager.h"
#include "Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Safe_Delete.h"


namespace Ensum
{
	namespace Components
	{
		SceneManager::SceneManager() : 
			Manager(new SceneData, _entManager),
			_datap(((SceneData*)(_data)))
		{
			_Allocate(10);
		}


		SceneManager::~SceneManager()
		{
			for (auto& scene : (*_entityToIndex))
			{
				SAFE_DELETE(_datap->scenePtr[scene.second]);
			}
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

			if (_datap->used >= _datap->allocated)
				_Allocate(static_cast<uint32_t>(_datap->allocated * 1.5f) + 10);

			const Entity& ent = scene->GetEntity();
			Utils::ConsoleLog::DumpToConsole("Creating Scene with id: %d", ent.ID);
			
			uint32_t index = (*_entityToIndex)[ent] = (uint32_t)_entityToIndex->size();
			_datap->entity[index] = ent;
			_datap->scenePtr[index] = scene;
			_datap->sceneUpdate[index] = true;
			_datap->used++;
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
				_datap->sceneUpdate[find->second] = update;
			}
		}
		const void SceneManager::Frame()
		{
			_GC();

			for (uint32_t i = 0; i < _datap->used; i++)
			{
				if (_datap->sceneUpdate[i]) _datap->scenePtr[i]->Frame();
			}
			return void();
		}
		EntityManager& SceneManager::GetEntityManager()
		{
			return _entManager;
		}
		const void SceneManager::_Allocate(uint32_t size)
		{
			if (size <= _datap->allocated) Exception("Alloc should only increase.");

			SceneData* new_data = new SceneData;
			size_t bytes = static_cast<size_t>(size * (sizeof(Entity) + sizeof(Scene*) + sizeof(bool)));
			new_data->buffer = operator new(bytes);
			new_data->used = _datap->used;
			new_data->allocated = size;

			new_data->entity = (Entity*)(new_data->buffer);
			new_data->scenePtr = (Scene**)(new_data->entity + size);
			new_data->sceneUpdate = (bool *)(new_data->scenePtr + size);

			memcpy(new_data->entity, _datap->entity, _datap->used * sizeof(Entity));
			memcpy(new_data->scenePtr, _datap->scenePtr, _datap->used * sizeof(Scene*));
			memcpy(new_data->sceneUpdate, _datap->sceneUpdate, _datap->used * sizeof(bool));


			operator delete(_datap->buffer);
			delete _data;
			_data = new_data;
			_datap = (SceneData*)_data;
		}
		const void SceneManager::_Destroy(uint32_t index)
		{
			uint32_t last = _datap->used - 1;
			const Entity& e = _datap->entity[index];
			const Entity& last_e = _datap->entity[last];

			Utils::ConsoleLog::DumpToConsole("Deleting Scene with id: %d", e.ID);

			_datap->entity[index] = _datap->entity[last];
			SAFE_DELETE(_datap->scenePtr[index]);
			_datap->scenePtr[index] = _datap->scenePtr[last];
			_datap->sceneUpdate[index] = _datap->sceneUpdate[last];

			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_datap->used--;
		}
	}
}