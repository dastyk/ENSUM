#include "Ensum_components\StaticMeshManager.h"
#include "Exception.h"

namespace Ensum
{
	namespace Components
	{
		StaticMeshManager::StaticMeshManager(EntityManager& entManager) :
			Manager(new MeshComponent, entManager),
			_datap(((MeshComponent*)(_data)))
		{
			_loadedMeshes = new std::unordered_map<uint32_t, FileHandler::MeshData>;
			_Allocate(10);
		}


		StaticMeshManager::~StaticMeshManager()
		{
			for (uint32_t i = 0; i < _datap->used; i++)
			{
				delete[] _datap->indexStart[i];
				delete[] _datap->indexCount[i];
				delete[] _datap->visible[i];
			}
			delete _loadedMeshes;
		}
		const void StaticMeshManager::CreateStaticMesh(const Entity & entity, const char * path)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
				return;

			if (_data->used + 1 >= _data->allocated)
				_Allocate(_data->allocated * 2 + 10);

			uint32_t index = (*_entityToIndex)[entity] = static_cast<uint32_t>(_entityToIndex->size());
			_datap->entity[index] = entity;

			uint32_t hash = string(path).GetHash();
			auto& loaded = _loadedMeshes->find(hash);

			if (loaded == _loadedMeshes->end())
			{
				FileHandler::Mesh mesh(path);
				(*_loadedMeshes)[hash] = Graphics::Graphics::CreateMeshData(mesh);
			}

			auto& meshData = (*_loadedMeshes)[hash];
			_datap->bufferID[index] = meshData.bufferID;
			_datap->numSubMeshes[index] = meshData.numSubMeshes;
			_datap->indexStart[index] = new uint64_t[meshData.numSubMeshes];
			_datap->indexCount[index] = new uint64_t[meshData.numSubMeshes];
			_datap->visible[index] = new bool[meshData.numSubMeshes];
			memcpy(_datap->indexStart[index], meshData.indexStart, meshData.numSubMeshes * sizeof(uint64_t));
			memcpy(_datap->indexCount[index], meshData.indexCount, meshData.numSubMeshes * sizeof(uint64_t));
			memset(_datap->visible[index], 0, meshData.numSubMeshes * sizeof(bool));
			
			_datap->used++;
			return void();
		}
		const void StaticMeshManager::_Allocate(uint32_t size)
		{
			if (size <= _datap->allocated) Exception("Alloc should only increase.");

			MeshComponent* new_data = new MeshComponent;
			size_t bytes = static_cast<size_t>(
				size * (
					sizeof(Entity) + 
					sizeof(uint32_t)*2 + 
					sizeof(uint64_t*)*2 + 
					sizeof(bool*)));

			new_data->buffer = operator new(bytes);
			new_data->used = _datap->used;
			new_data->allocated = size;

			new_data->entity = (Entity*)(new_data->buffer);
			new_data->bufferID = (uint32_t*)(new_data->entity + static_cast<size_t>(size));
			new_data->numSubMeshes = (uint32_t*)(new_data->bufferID + static_cast<size_t>(size));
			new_data->indexStart = (uint64_t**)(new_data->numSubMeshes + static_cast<size_t>(size));
			new_data->indexCount = (uint64_t**)(new_data->indexStart + static_cast<size_t>(size));
			new_data->visible = (bool**)(new_data->indexCount + static_cast<size_t>(size));


			memcpy(new_data->entity, _datap->entity, _datap->used * sizeof(Entity));
			memcpy(new_data->bufferID, _datap->bufferID, _datap->used * sizeof(uint32_t));
			memcpy(new_data->numSubMeshes, _datap->numSubMeshes, _datap->used * sizeof(uint32_t));
			memcpy(new_data->indexStart, _datap->indexStart, _datap->used * sizeof(uint64_t*));
			memcpy(new_data->indexCount, _datap->indexCount, _datap->used * sizeof(uint64_t*));
			memcpy(new_data->visible, _datap->visible, _datap->used * sizeof(bool*));


			operator delete(_datap->buffer);
			delete _data;
			_data = new_data;
			_datap = (MeshComponent*)_data;
		}
		const void StaticMeshManager::_Destroy(uint32_t index)
		{
			uint32_t last = _datap->used - 1;
			const Entity& e = _datap->entity[index];
			const Entity& last_e = _datap->entity[last];

			//_datap->meshData[index]->subs--;
			_datap->entity[index] = _datap->entity[last];
			_datap->bufferID[index] = _datap->bufferID[last];
			_datap->numSubMeshes[index] = _datap->numSubMeshes[last];
			delete[] _datap->indexStart[index];
			delete[] _datap->indexCount[index];
			delete[] _datap->visible[index];
			_datap->indexStart[index] = _datap->indexStart[last];
			_datap->indexCount[index] = _datap->indexCount[last];
			_datap->visible[index] = _datap->visible[last];



			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_datap->used--;
		}
	}
}