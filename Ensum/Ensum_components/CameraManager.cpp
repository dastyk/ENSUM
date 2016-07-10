#include "Ensum_components\CameraManager.h"
#include "Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Options.h"


#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif

namespace Ensum
{
	namespace Components
	{

		CameraManager::CameraManager(EntityManager& entManager, TransformManager& transformManager, Input::Input& input) :
			Manager(new CameraData, entManager),
			_transformManager(transformManager),
			_input(input),
			_datap(((CameraData*)(_data)))
		{
			Utils::Options::Subscribe(Delegate<const void()>::Make<CameraManager, &CameraManager::_OnOptionsChange>(this));
			_Allocate(10);
		}


		CameraManager::~CameraManager()
		{
			Utils::Options::UnSubscribe(Delegate<const void()>::Make<CameraManager, &CameraManager::_OnOptionsChange>(this));
		}
		const void CameraManager::CreateControllableCamera(const Entity & entity)
		{
			auto find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
				return;

			if (_data->used + 1 >= _data->allocated)
				_Allocate(_data->allocated * 2 + 10);

			if (!_entityManager.Alive(entity))
			{
				Utils::ConsoleLog::DumpToConsole("Tried to bind camera to a dead entity. Entity: %d", entity.ID);
				return;
			}

			

			Utils::ConsoleLog::DumpToConsole("Creating camera for Entity: %d", entity.ID);
			
			_entityManager.AddDeleteCallback(entity, Delegate<const void(const Entity& entity)>::Make<Manager, &Manager::Destroy>(this));
			
			uint32_t index = (*_entityToIndex)[entity] = static_cast<uint32_t>(_entityToIndex->size());
			_datap->entity[index] = entity;

			using namespace DirectX;
			
			_datap->fov[index] = XMConvertToRadians((float)Utils::Options::GetIntegerOption("Graphics", "FOV", 90));
			_datap->aspect[index] = (float)Utils::Options::GetRealOption("Graphics", "AspectRatio", 1.25);
			_datap->nearp[index] = (float)Utils::Options::GetRealOption("Graphics", "NearPlane", 0.1);
			_datap->viewDistance[index] = (float)Utils::Options::GetRealOption("Graphics", "DrawDistance", 25.0);

			_datap->controllable[index] = true;
			XMMATRIX projM = DirectX::XMMatrixPerspectiveFovLH(_datap->fov[index], _datap->aspect[index], _datap->nearp[index], _datap->viewDistance[index]);
			DirectX::XMStoreFloat4x4(&_datap->projectionMatrix[index], projM);

			uint32_t tindex;
			auto find2 = _transformManager._entityToIndex->find(entity);
			if (find2 == _transformManager._entityToIndex->end())
			{
				tindex = _transformManager._CreateTransform(entity);
			}
			else
				tindex = find2->second;


			XMVECTOR position = _transformManager._GetPosition(tindex);
			XMVECTOR up = _transformManager._GetUp(tindex);
			XMVECTOR forward = _transformManager._GetForward(tindex);

			XMVECTOR lookAt;
			XMMATRIX rotationMatrix;

			// Translate the rotated camera position to the location of the viewer.
			lookAt = position + forward;

			// Finally create the view matrix from the three updated vectors.
			XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

			DirectX::XMStoreFloat4x4(&_datap->viewMatrix[index], viewMatrix);
			DirectX::XMStoreFloat4x4(&_datap->viewProjectionMatrix[index], viewMatrix * projM);

			_datap->used++;
		}
		const void CameraManager::CreateUnControllableCamera(const Entity & entity)
		{
			auto find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
				return;

			if (_data->used + 1 >= _data->allocated)
				_Allocate(_data->allocated * 2 + 10);

			if (!_entityManager.Alive(entity))
			{
				Utils::ConsoleLog::DumpToConsole("Tried to bind camera to a dead entity. Entity: %d", entity.ID);
				return;
			}



			Utils::ConsoleLog::DumpToConsole("Creating camera for Entity: %d", entity.ID);

			_entityManager.AddDeleteCallback(entity, Delegate<const void(const Entity& entity)>::Make<Manager, &Manager::Destroy>(this));

			uint32_t index = (*_entityToIndex)[entity] = static_cast<uint32_t>(_entityToIndex->size());
			_datap->entity[index] = entity;

			using namespace DirectX;

			_datap->fov[index] = XMConvertToRadians((float)Utils::Options::GetIntegerOption("Graphics", "FOV", 90));
			_datap->aspect[index] = (float)Utils::Options::GetRealOption("Graphics", "AspectRatio", 1.25);
			_datap->nearp[index] = (float)Utils::Options::GetRealOption("Graphics", "NearPlane", 0.1);
			_datap->viewDistance[index] = (float)Utils::Options::GetRealOption("Graphics", "DrawDistance", 25.0);

			_datap->controllable[index] = false;

			XMMATRIX projM = DirectX::XMMatrixPerspectiveFovLH(_datap->fov[index], _datap->aspect[index], _datap->nearp[index], _datap->viewDistance[index]);
			DirectX::XMStoreFloat4x4(&_datap->projectionMatrix[index], projM);

			uint32_t tindex;
			auto find2 = _transformManager._entityToIndex->find(entity);
			if (find2 == _transformManager._entityToIndex->end())
			{
				tindex = _transformManager._CreateTransform(entity);
			}
			else
				tindex = find2->second;


			XMVECTOR position = _transformManager._GetPosition(tindex);
			XMVECTOR up = _transformManager._GetUp(tindex);
			XMVECTOR forward = _transformManager._GetForward(tindex);

			XMVECTOR lookAt;
			XMMATRIX rotationMatrix;

			// Translate the rotated camera position to the location of the viewer.
			lookAt = position + forward;

			// Finally create the view matrix from the three updated vectors.
			XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

			DirectX::XMStoreFloat4x4(&_datap->viewMatrix[index], viewMatrix);
			DirectX::XMStoreFloat4x4(&_datap->viewProjectionMatrix[index], viewMatrix * projM);

			_datap->used++;
		}
		const void CameraManager::_Allocate(uint32_t size)
		{
			using namespace DirectX;
			if (size <= _datap->allocated) Exception("Alloc should only increase.");

			CameraData* new_data = new CameraData;
			size_t bytes = static_cast<size_t>(size * (sizeof(Entity) + sizeof(uint32_t) * 4 + sizeof(XMFLOAT4X4) * 2 + sizeof(XMFLOAT3) * 7 + sizeof(bool)));
			new_data->buffer = operator new(bytes);
			new_data->used = _datap->used;
			new_data->allocated = size;

			new_data->entity = (Entity*)(new_data->buffer);

			new_data->fov = (float*)(new_data->entity + static_cast<size_t>(size));
			new_data->aspect = (float*)(new_data->fov + static_cast<size_t>(size));
			new_data->nearp = (float*)(new_data->aspect + static_cast<size_t>(size));
			new_data->viewDistance = (float*)(new_data->nearp + static_cast<size_t>(size));
		
			new_data->controllable = (bool*)(new_data->viewDistance + static_cast<size_t>(size));

			new_data->viewMatrix = (XMFLOAT4X4*)(new_data->controllable + static_cast<size_t>(size));
			new_data->projectionMatrix = (XMFLOAT4X4*)(new_data->viewMatrix + static_cast<size_t>(size));
			new_data->viewProjectionMatrix = (XMFLOAT4X4*)(new_data->projectionMatrix + static_cast<size_t>(size));


			memcpy(new_data->entity, _datap->entity, _datap->used * sizeof(Entity));

			memcpy(new_data->fov, _datap->fov, _datap->used * sizeof(float));
			memcpy(new_data->aspect, _datap->aspect, _datap->used * sizeof(float));
			memcpy(new_data->nearp, _datap->nearp, _datap->used * sizeof(float));
			memcpy(new_data->viewDistance, _datap->viewDistance, _datap->used * sizeof(float));

			memcpy(new_data->controllable, _datap->controllable, _datap->used * sizeof(bool));
			
			memcpy(new_data->viewMatrix, _datap->viewMatrix, _datap->used * sizeof(XMFLOAT4X4));
			memcpy(new_data->projectionMatrix, _datap->projectionMatrix, _datap->used * sizeof(XMFLOAT4X4));
			memcpy(new_data->viewProjectionMatrix, _datap->viewProjectionMatrix, _datap->used * sizeof(XMFLOAT4X4));
		


			operator delete(_datap->buffer);
			delete _data;
			_data = new_data;
			_datap = (CameraData*)_data;
		}
		const void CameraManager::_Destroy(uint32_t index)
		{
			uint32_t last = _datap->used - 1;
			const Entity& e = _datap->entity[index];
			const Entity& last_e = _datap->entity[last];

			_datap->entity[index] = _datap->entity[last];

			_datap->fov[index] = _datap->fov[last];
			_datap->aspect[index] = _datap->aspect[last];
			_datap->nearp[index] = _datap->nearp[last];
			_datap->viewDistance[index] = _datap->viewDistance[last];

			_datap->controllable[index] = _datap->controllable[last];
			
			_datap->viewMatrix[index] = _datap->viewMatrix[last];
			_datap->projectionMatrix[index] = _datap->projectionMatrix[last];
			_datap->viewProjectionMatrix[index] = _datap->viewProjectionMatrix[last];
		

			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_datap->used--;

			index = _transformManager._entityToIndex->find(_datap->entity[index])->second;
			_transformManager._Destroy(index);
		}
		const void CameraManager::Frame()
		{
			_GC();
			DirectX::XMFLOAT3 direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			bool change = false;
			if (_input.IsKeyDown("Forward"))
				direction.x += 1.0f;
			if (_input.IsKeyDown("Back"))
				direction.x -= 1.0f;
			if (_input.IsKeyDown("Right"))
				direction.y += 1.0f;
			if (_input.IsKeyDown("Left"))
				direction.y -= 1.0f;
			if (_input.IsKeyDown("Up"))
				direction.y += 1.0f;
			if (_input.IsKeyDown("Down"))
				direction.y -= 1.0f;
			if (fabs(direction.x) > 0.0001 || fabs(direction.y) > 0.0001 || fabs(direction.z) > 0.0001)
				change = true;
		
			int x = 0, y = 0;
			_input.GetMouseDiff(x, y);



			for (auto& c : *_entityToIndex)
			{
				if (_datap->controllable[c.second])
				{
					uint32_t index = (*_transformManager._entityToIndex)[c.first];
					if (change)
					{
						_transformManager._Move(index, direction);
					}
					if (x != 0 || y != 0)
					{
						_transformManager._Rotate(index, DirectX::XMFLOAT3(x  * 0.1f, y*0.1f, 0.0f));
					}
				}
			}
		}
		const void CameraManager::_OnOptionsChange()
		{
			return void();
		}
	}
}