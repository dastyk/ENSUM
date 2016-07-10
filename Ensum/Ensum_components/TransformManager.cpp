#include "Ensum_components\TransformManager.h"
#include "Exception.h"

#include "Ensum_utils\ConsoleLog.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif

namespace Ensum
{
	namespace Components
	{
		TransformManager::TransformManager(EntityManager& entManager) :
			Manager(new TransformComponents, entManager),
			_datap(((TransformComponents*)(_data)))
		{
			_Allocate(10);
		}


		TransformManager::~TransformManager()
		{
		}
		const void TransformManager::CreateTransform(const Entity & entity)
		{
			auto find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
				return;
			if (!_entityManager.Alive(entity))
			{
				Utils::ConsoleLog::DumpToConsole("Tried to bind transformcomponent to a dead entity. Entity: %d", entity.ID);
				return;
			}

			Utils::ConsoleLog::DumpToConsole("Creating transformcomponent for Entity: %d", entity.ID);

			_CreateTransform(entity);
			
		}
		const void TransformManager::BindChild(const Entity & parent, const Entity & child, bool relativeToParent)
		{
			auto parenti = _entityToIndex->find(parent);
			if (parenti == _entityToIndex->end())
				return;

			auto childi = _entityToIndex->find(child);
			if (childi == _entityToIndex->end())
				return;

			_BindChild(parenti->second, childi->second, relativeToParent);

			return void();
		}
		const void TransformManager::UnbindChild(const Entity & child)
		{
			auto childi = _entityToIndex->find(child);
			if (childi == _entityToIndex->end())
				return;

			_UnbindChild(childi->second);

		}
		const void TransformManager::Move(const Entity & entity, const DirectX::XMFLOAT3& direction)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;

			_Move(find->second, direction);
		}
		const void TransformManager::MoveForward(const Entity & entity, const float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveForward(find->second, amount);
		}
		const void TransformManager::MoveBackward(const Entity & entity, const float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return; 		
			_MoveBackward(find->second, amount);
		}
		const void TransformManager::MoveRight(const Entity & entity, const float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveRight(find->second, amount);
		}
		const void TransformManager::MoveLeft(const Entity & entity, const float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveLeft(find->second, amount);
		}
		const void TransformManager::MoveUp(const Entity & entity, const float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveUp(find->second, amount);
		}
		const void TransformManager::MoveDown(const Entity & entity, const float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveDown(find->second, amount);
		}
		const void TransformManager::MoveAlongVector(const Entity & entity, const DirectX::XMVECTOR amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveAlongVector(find->second, amount);
		}
		const void TransformManager::MoveAlongVector(const Entity & entity, DirectX::XMVECTOR dir, float amount)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_MoveAlongVector(find->second, dir, amount);
		}
		const void TransformManager::Rotate(const Entity & entity, const DirectX::XMFLOAT3 & radians)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_Rotate(find->second, radians);
		}
		const void TransformManager::RotateYaw(const Entity & entity, const float radians)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_RotateYaw(find->second, radians);
		}
		const void TransformManager::RotatePitch(const Entity & entity, const float radians)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_RotatePitch(find->second, radians);
		}
		const void TransformManager::RotateRoll(const Entity & entity, const float radians)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_RotateRoll(find->second, radians);
		}
		const void TransformManager::SetPosition(const Entity & entity, const DirectX::XMFLOAT3 & position, bool relativeToParent)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetPosition(find->second, position, relativeToParent);
		}
		const void TransformManager::SetPosition(const Entity & entity, const DirectX::XMVECTOR & position, bool relativeToParent)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetPosition(find->second, position, relativeToParent);
		}
		const void TransformManager::SetRotation(const Entity & entity, const DirectX::XMFLOAT3 & rotation)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetRotation(find->second, rotation);
		}
		const void TransformManager::SetRotation(const Entity & entity, const DirectX::XMVECTOR & rotation)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetRotation(find->second, rotation);
		}
		const void TransformManager::SetScale(const Entity & entity, const DirectX::XMFLOAT3 & scale, bool relativeToParent)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetScale(find->second, scale, relativeToParent);
		}
		const void TransformManager::SetScale(const Entity & entity, const DirectX::XMVECTOR & scale, bool relativeToParent)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetScale(find->second, scale, relativeToParent);
		}
		const void TransformManager::SetForward(const Entity & entity, const DirectX::XMFLOAT3 & forward, bool relativeToParent)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetForward(find->second, forward, relativeToParent);
		}
		const void TransformManager::SetForward(const Entity & entity, const DirectX::XMVECTOR & forward, bool relativeToParent)
		{
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return;
			_SetForward(find->second, forward, relativeToParent);
		}

		const DirectX::XMVECTOR TransformManager::GetPosition(const Entity & entity)
		{
			using namespace DirectX;
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			return _GetPosition(find->second);	
		}
		const DirectX::XMVECTOR TransformManager::GetRotation(const Entity & entity)
		{
			using namespace DirectX;
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			return _GetRotation(find->second);
		}
		const DirectX::XMVECTOR TransformManager::GetScale(const Entity & entity)
		{
			using namespace DirectX;
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
			return _GetScale(find->second);
		}
		const DirectX::XMVECTOR TransformManager::GetForward(const Entity & entity)
		{
			using namespace DirectX;
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			return _GetForward(find->second);
		}
		const DirectX::XMVECTOR TransformManager::GetRight(const Entity & entity)
		{
			using namespace DirectX;
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			return _GetRight(find->second);
		}
		const DirectX::XMVECTOR TransformManager::GetUp(const Entity & entity)
		{
			using namespace DirectX;
			auto find = _entityToIndex->find(entity);
			if (find == _entityToIndex->end())
				return XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			return _GetUp(find->second);
		}
		const void TransformManager::_Allocate(uint32_t size)
		{
			using namespace DirectX;
			if (size <= _datap->allocated) Exception("Alloc should only increase.");

			TransformComponents* new_data = new TransformComponents;
			size_t bytes = static_cast<size_t>(size * (sizeof(Entity) + sizeof(uint32_t) * 4 + sizeof(XMFLOAT4X4) * 2 + sizeof(XMFLOAT3) * 7 + sizeof(bool)));
			new_data->buffer = operator new(bytes);
			new_data->used = _datap->used;
			new_data->allocated = size;

			new_data->entity = (Entity*)(new_data->buffer);

			new_data->Parent = (uint32_t*)(new_data->entity + static_cast<size_t>(size));
			new_data->FirstChild = (uint32_t*)(new_data->Parent + static_cast<size_t>(size));
			new_data->PrevSibling = (uint32_t*)(new_data->FirstChild + static_cast<size_t>(size));
			new_data->NextSibling = (uint32_t*)(new_data->PrevSibling + static_cast<size_t>(size));

			new_data->Local = (XMFLOAT4X4*)(new_data->NextSibling + static_cast<size_t>(size));
			new_data->World = (XMFLOAT4X4*)(new_data->Local + static_cast<size_t>(size));

			new_data->PositionL = (XMFLOAT3*)(new_data->World + static_cast<size_t>(size));
			new_data->Rotation = (XMFLOAT3*)(new_data->PositionL + static_cast<size_t>(size));
			new_data->Scale = (XMFLOAT3*)(new_data->Rotation + static_cast<size_t>(size));

			new_data->Forward = (XMFLOAT3*)(new_data->Scale + static_cast<size_t>(size));
			new_data->Up = (XMFLOAT3*)(new_data->Forward + static_cast<size_t>(size));
			new_data->Right = (XMFLOAT3*)(new_data->Up + static_cast<size_t>(size));

			memcpy(new_data->entity, _datap->entity, _datap->used * sizeof(Entity));

			memcpy(new_data->Parent, _datap->Parent, _datap->used * sizeof(uint32_t));
			memcpy(new_data->FirstChild, _datap->FirstChild, _datap->used * sizeof(uint32_t));
			memcpy(new_data->PrevSibling, _datap->PrevSibling, _datap->used * sizeof(uint32_t));
			memcpy(new_data->NextSibling, _datap->NextSibling, _datap->used * sizeof(uint32_t));

			memcpy(new_data->Local, _datap->Local, _datap->used * sizeof(XMFLOAT4X4));
			memcpy(new_data->World, _datap->World, _datap->used * sizeof(XMFLOAT4X4));
			
			memcpy(new_data->PositionL, _datap->PositionL, _datap->used * sizeof(XMFLOAT3));
			memcpy(new_data->Rotation, _datap->Rotation, _datap->used * sizeof(XMFLOAT3));
			memcpy(new_data->Scale, _datap->Scale, _datap->used * sizeof(XMFLOAT3));

			memcpy(new_data->Forward, _datap->Forward, _datap->used * sizeof(XMFLOAT3));
			memcpy(new_data->Up, _datap->Up, _datap->used * sizeof(XMFLOAT3));
			memcpy(new_data->Right, _datap->Right, _datap->used * sizeof(XMFLOAT3));


			operator delete(_datap->buffer);
			delete _data;
			_data = new_data;
			_datap = (TransformComponents*)_data;
		}
		const void TransformManager::_Destroy(uint32_t index)
		{
			uint32_t last = _datap->used - 1;
			const Entity& e = _datap->entity[index];
			const Entity& last_e = _datap->entity[last];

			_datap->entity[index] = _datap->entity[last];

			_datap->Parent[index] = _datap->Parent[last];
			_datap->FirstChild[index] = _datap->FirstChild[last];
			_datap->PrevSibling[index] = _datap->PrevSibling[last];
			_datap->NextSibling[index] = _datap->NextSibling[last];

			_datap->Local[index] = _datap->Local[last];
			_datap->World[index] = _datap->World[last];

			_datap->PositionL[index] = _datap->PositionL[last];
			_datap->Rotation[index] = _datap->Rotation[last];
			_datap->Scale[index] = _datap->Scale[last];

			_datap->Forward[index] = _datap->Forward[last];
			_datap->Up[index] = _datap->Up[last];
			_datap->Right[index] = _datap->Right[last];


			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_datap->used--;
		}
		const uint32_t TransformManager::_CreateTransform(const Entity& entity)
		{
			if (_data->used + 1 >= _data->allocated)
				_Allocate(_data->allocated * 2 + 10);

	
			uint32_t index = (*_entityToIndex)[entity] = static_cast<uint32_t>(_entityToIndex->size());
			_datap->entity[index] = entity;

			using namespace DirectX;

			XMStoreFloat4x4(&(_datap->Local[index]), XMMatrixIdentity());
			XMStoreFloat4x4(&(_datap->World[index]), XMMatrixIdentity());

			_datap->Parent[index] = -1;
			_datap->FirstChild[index] = -1;
			_datap->PrevSibling[index] = -1;
			_datap->NextSibling[index] = -1;

			_datap->PositionL[index] = XMFLOAT3(0.0f, 0.0f, 0.0f);
			_datap->Rotation[index] = XMFLOAT3(0.0f, 0.0f, 0.0f);
			_datap->Scale[index] = XMFLOAT3(1.0f, 1.0f, 1.0f);

			_datap->Forward[index] = XMFLOAT3(0.0f, 0.0f, 1.0f);
			_datap->Up[index] = XMFLOAT3(0.0f, 1.0f, 0.0f);
			_datap->Right[index] = XMFLOAT3(1.0f, 0.0f, 0.0f);

			_datap->used++;

			return index;
		}
		const void TransformManager::_BindChild(uint32_t parent, uint32_t child, bool relativeToParent)
		{

			if (_datap->Parent[child] != -1)
			{
				// Child already had a parent.
				_UnbindChild(child);
			}
			_datap->Parent[child] = parent;

			if (_datap->FirstChild[parent] == -1)
			{
				// Did not have any children, so just bind it.
				_datap->FirstChild[parent] = child;


			}
			else
			{
				// Parent had children, so loop until we find a empty spot.
				uint32_t pspot = -1;
				uint32_t spot = child;
				while (spot != -1)
				{
					pspot = spot;
					spot = _datap->NextSibling[spot];
				}

				// We now have a spot, so put the child here.
				_datap->PrevSibling[spot] = pspot;
				_datap->NextSibling[pspot] = spot;


			}
			if (!relativeToParent)
			{
				// Transform the child to it's parents local space.
				_TransformToParentLocal(parent, child);
			}

		}
		const void TransformManager::_UnbindChild(uint32_t child)
		{

			uint32_t parent = _datap->Parent[child];

			_datap->Parent[child] = -1;
			if (_datap->NextSibling[child] == -1)
			{
				uint32_t ps = _datap->PrevSibling[child];
				if (ps != -1)
					_datap->NextSibling[ps] = -1;

				_datap->PrevSibling[child] = -1;
			}
			else
			{
				uint32_t ps = _datap->PrevSibling[child];
				uint32_t ns = _datap->NextSibling[child];

				_datap->NextSibling[ps] = ns;
				_datap->PrevSibling[ns] = ps;

				_datap->NextSibling[child] = -1;
				_datap->PrevSibling[child] = -1;
			}

			if (parent != -1)
			{
				// Lastly transform the local pos of the child, so that no teleporting happens.
				_TransformFromParentLocal(parent, child);
			}

		}
		
		const void TransformManager::_Move(uint32_t index, const DirectX::XMFLOAT3& direction)
		{
			using namespace DirectX;

			XMVECTOR Forward = XMLoadFloat3(&_datap->Forward[index]);
			XMVECTOR Right = XMLoadFloat3(&_datap->Right[index]);
			XMVECTOR Up = XMLoadFloat3(&_datap->Up[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position += Forward * direction.x;
			position += Right * direction.y;
			position += Up * direction.z;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}

		inline const void TransformManager::_MoveForward(uint32_t index, const float amount)
		{
			using namespace DirectX;

			XMVECTOR forward = XMLoadFloat3(&_datap->Forward[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position += forward * amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline const void TransformManager::_MoveBackward(uint32_t index, const float amount)
		{
			using namespace DirectX;

			XMVECTOR forward = XMLoadFloat3(&_datap->Forward[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position -= forward * amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline 	const void TransformManager::_MoveRight(uint32_t index, const float amount)
		{
			using namespace DirectX;
			XMVECTOR Right = XMLoadFloat3(&_datap->Right[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position += Right * amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline const void TransformManager::_MoveLeft(uint32_t index, const float amount)
		{
			using namespace DirectX;
			XMVECTOR Right = XMLoadFloat3(&_datap->Right[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position -= Right * amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline const void TransformManager::_MoveUp(uint32_t index, const float amount)
		{
			using namespace DirectX;
			XMVECTOR Up = XMLoadFloat3(&_datap->Up[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position += Up * amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline const void TransformManager::_MoveDown(uint32_t index, const float amount)
		{
			using namespace DirectX;
			XMVECTOR Up = XMLoadFloat3(&_datap->Up[index]);
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position -= Up * amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline 	const void TransformManager::_MoveAlongVector(uint32_t index, const DirectX::XMVECTOR amount)
		{
			using namespace DirectX;
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position += amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline const void TransformManager::_MoveAlongVector(uint32_t index, DirectX::XMVECTOR dir, float amount)
		{
			using namespace DirectX;
			XMVECTOR position = XMLoadFloat3(&_datap->PositionL[index]);

			position += dir*amount;

			XMStoreFloat3(&_datap->PositionL[index], position);

			_Transform(index);
		}
		inline const void TransformManager::_Rotate(uint32_t index, const DirectX::XMFLOAT3 & radians)
		{
			using namespace DirectX;

			float& pitch = _datap->Rotation[index].x;
			pitch = fmod(pitch + radians.x, XM_2PI);
			float& yaw = _datap->Rotation[index].y;
			yaw = fmod(yaw + radians.y, XM_2PI);
			float& roll = _datap->Rotation[index].z;
			roll = fmod(roll + radians.z, XM_2PI);

			_Transform(index);
		}
		inline const void TransformManager::_RotateYaw(uint32_t index, const float radians)
		{
			using namespace DirectX;
			float& yaw = _datap->Rotation[index].y;
			yaw = fmod(yaw + radians, XM_2PI);

			_Transform(index);
		}
		inline const void TransformManager::_RotatePitch(uint32_t index, const float radians)
		{
			using namespace DirectX;
			float& pitch = _datap->Rotation[index].x;
			pitch = fmod(pitch + radians, XM_2PI);

			_Transform(index);
		}
		inline const void TransformManager::_RotateRoll(uint32_t index, const float radians)
		{
			using namespace DirectX;
			float& roll = _datap->Rotation[index].z;
			roll = fmod(roll + radians, XM_2PI);

			_Transform(index);
		}	
		inline const void TransformManager::_SetPosition(uint32_t index, const DirectX::XMFLOAT3 & position, bool relativeToParent)
		{
			using namespace DirectX;
			if (relativeToParent)
			{
				_datap->PositionL[index] = position;
				_Transform(index);
			}
			else
			{
				uint32_t parent = _datap->Parent[index];
				if (parent != -1)
				{
					XMMATRIX parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
					parentWorld = XMMatrixInverse(nullptr, parentWorld);
					XMVECTOR pos = XMLoadFloat3(&position);
					pos = XMVector3TransformCoord(pos, parentWorld);
					XMStoreFloat3(&_datap->PositionL[index], pos);
				}
				else
				{
					_datap->PositionL[index] = position;
				}			
				_Transform(index);
			}
			
		}
		inline const void TransformManager::_SetPosition(uint32_t index, const DirectX::XMVECTOR & position, bool relativeToParent)
		{
			using namespace DirectX;
			if (relativeToParent)
			{
				XMStoreFloat3(&_datap->PositionL[index], position);
				_Transform(index);
			}
			else
			{
				uint32_t parent = _datap->Parent[index];
				if (parent != -1)
				{
					XMMATRIX parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
					parentWorld = XMMatrixInverse(nullptr, parentWorld);
					XMVECTOR pos = XMVector3TransformCoord(position, parentWorld);
					XMStoreFloat3(&_datap->PositionL[index], pos);
				}
				else
				{
					XMStoreFloat3(&_datap->PositionL[index], position);
				}
				_Transform(index);
			}
		}
		inline const void TransformManager::_SetRotation(uint32_t index, const DirectX::XMFLOAT3 & rotation)
		{
			_datap->Rotation[index] = rotation;
			_Transform(index);
		}
		inline const void TransformManager::_SetRotation(uint32_t index, const DirectX::XMVECTOR & rotation)
		{
			using namespace DirectX;
			XMStoreFloat3(&_datap->Rotation[index], rotation);
			_Transform(index);
		}
		inline const void TransformManager::_SetScale(uint32_t index, const DirectX::XMFLOAT3 & scale, bool relativeToParent)
		{
			using namespace DirectX;
			if (relativeToParent)
			{
				_datap->Scale[index] = scale;
				_Transform(index);
			}
			else
			{
				uint32_t parent = _datap->Parent[index];
				if (parent != -1)
				{
					XMVECTOR pscale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
					do
					{
						pscale *= XMLoadFloat3(&_datap->Scale[parent]);
						parent = _datap->Parent[parent];
					} while (parent != -1);

					XMVECTOR scale = XMLoadFloat3(&_datap->Scale[index]);
					scale = XMVectorDivide(scale, pscale);
					XMStoreFloat3(&_datap->Scale[index], scale);
				}
				else
				{
					_datap->Scale[index] = scale;
				}
				_Transform(index);
			}
		}
		inline const void TransformManager::_SetScale(uint32_t index, const DirectX::XMVECTOR & scale, bool relativeToParent)
		{
			using namespace DirectX;
			if (relativeToParent)
			{
				XMStoreFloat3(&_datap->Scale[index], scale);
				_Transform(index);
			}
			else
			{
				uint32_t parent = _datap->Parent[index];
				if (parent != -1)
				{
					XMVECTOR pscale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
					do
					{
						pscale *= XMLoadFloat3(&_datap->Scale[parent]);
						parent = _datap->Parent[parent];
					} while (parent != -1);

					XMVECTOR scale = XMLoadFloat3(&_datap->Scale[index]);
					scale = XMVectorDivide(scale, pscale);
					XMStoreFloat3(&_datap->Scale[index], scale);
				}
				else
				{
					XMStoreFloat3(&_datap->Scale[index], scale);
				}
				_Transform(index);
			}
		}
		inline const void TransformManager::_SetForward(uint32_t index, const DirectX::XMFLOAT3 & forwardf, bool relativeToParent)
		{
			using namespace DirectX;

			XMVECTOR forward = XMLoadFloat3(&forwardf);

			XMVECTOR ndir = XMVector3Normalize(forward);
			if (!relativeToParent)
			{
				uint32_t parent = _datap->Parent[index];
				if (parent != -1)
				{
					XMMATRIX parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
					parentWorld = XMMatrixInverse(nullptr, parentWorld);
					ndir = XMVector3Normalize(XMVector3TransformNormal(ndir, parentWorld));
				}
			}

			XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			float angleY = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultUp))) < 0.0001))
			{
				XMVECTOR projToXZ = XMVector3Normalize(forward - (defaultUp * XMVector3Dot(defaultUp, forward)));
				angleY = XMVectorGetX(XMVector3AngleBetweenNormals(projToXZ, defaultForward));
			}
			float angleX = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultForward))) < 0.0001))
			{
				XMVECTOR projToXY = XMVector3Normalize(forward - (defaultForward * XMVector3Dot(defaultForward, forward)));
				angleX = XM_PIDIV2 - XMVectorGetX(XMVector3AngleBetweenNormals(projToXY, defaultUp));
			}

			_datap->Rotation[index].x = angleX;
			_datap->Rotation[index].y = angleY;

			_Transform(index);
		}
		inline const void TransformManager::_SetForward(uint32_t index, const DirectX::XMVECTOR & forward, bool relativeToParent)
		{
			using namespace DirectX;

			XMVECTOR ndir = XMVector3Normalize(forward);
			if (!relativeToParent)
			{
				uint32_t parent = _datap->Parent[index];
				if (parent != -1)
				{
					XMMATRIX parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
					parentWorld = XMMatrixInverse(nullptr, parentWorld);
					ndir = XMVector3Normalize(XMVector3TransformNormal(ndir, parentWorld));
				}
			}

			XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			float angleY = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultUp))) < 0.0001))
			{
				XMVECTOR projToXZ = XMVector3Normalize(forward - (defaultUp * XMVector3Dot(defaultUp, forward)));
				angleY = XMVectorGetX(XMVector3AngleBetweenNormals(projToXZ, defaultForward));
			}
			float angleX = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultForward))) < 0.0001))
			{
				XMVECTOR projToXY = XMVector3Normalize(forward - (defaultForward * XMVector3Dot(defaultForward, forward)));
				angleX = XM_PIDIV2 - XMVectorGetX(XMVector3AngleBetweenNormals(projToXY, defaultUp));
			}

			_datap->Rotation[index].x = angleX;
			_datap->Rotation[index].y = angleY;

			_Transform(index);
		}
		const DirectX::XMVECTOR TransformManager::_GetPosition(uint32_t index)
		{
			using namespace DirectX;
			if (_datap->Parent[index] != -1)
			{
				XMMATRIX worldMatrix = XMLoadFloat4x4(&_datap->World[index]);
				return XMVector3TransformCoord(XMVectorZero(), worldMatrix);
			}
			else
			{
				return XMLoadFloat3(&_datap->PositionL[index]);
			}
		}
		const DirectX::XMVECTOR TransformManager::_GetRotation(uint32_t index)
		{
			using namespace DirectX;
			return XMLoadFloat3(&_datap->PositionL[index]);
		}
		const DirectX::XMVECTOR TransformManager::_GetScale(uint32_t index)
		{
			using namespace DirectX;
			uint32_t parent = _datap->Parent[index];
			if (parent != -1)
			{
				XMVECTOR scale = XMLoadFloat3(&_datap->Scale[index]);
								do
				{
					scale *= XMLoadFloat3(&_datap->Scale[parent]);
					parent = _datap->Parent[parent];
				} while (parent != -1);

				return scale;
			}
			else
			{
				return XMLoadFloat3(&_datap->Scale[index]);
			}
		}
		const DirectX::XMVECTOR TransformManager::_GetForward(uint32_t index)
		{
			using namespace DirectX;
			if (_datap->Parent[index] != -1)
			{
				XMMATRIX worldMatrix = XMLoadFloat4x4(&_datap->World[index]);
				return XMVector3TransformNormal(XMVectorSet(0.0f,0.0f,1.0f,0.0f), worldMatrix);
			}
			else
			{
				return XMLoadFloat3(&_datap->Forward[index]);
			}
		}
		const DirectX::XMVECTOR TransformManager::_GetRight(uint32_t index)
		{
			using namespace DirectX;
			if (_datap->Parent[index] != -1)
			{
				XMMATRIX worldMatrix = XMLoadFloat4x4(&_datap->World[index]);
				return XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), worldMatrix);
			}
			else
			{
				return XMLoadFloat3(&_datap->Right[index]);
			}
		}
		const DirectX::XMVECTOR TransformManager::_GetUp(uint32_t index)
		{
			using namespace DirectX;
			if (_datap->Parent[index] != -1)
			{
				XMMATRIX worldMatrix = XMLoadFloat4x4(&_datap->World[index]);
				return XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), worldMatrix);
			}
			else
			{
				return XMLoadFloat3(&_datap->Up[index]);
			}
		}
		const void TransformManager::_TransformToParentLocal(uint32_t parent, uint32_t child)
		{
			using namespace DirectX;

			XMMATRIX parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
			XMMATRIX invParentWorld = XMMatrixInverse(nullptr, parentWorld);
			XMVECTOR pscale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
			do
			{
				pscale *= XMLoadFloat3(&_datap->Scale[parent]);
				parent = _datap->Parent[parent];
			} while (parent != -1);

			XMMATRIX childWorld = XMLoadFloat4x4(&_datap->Local[child]);
			XMVECTOR childWorldPos = XMLoadFloat3(&_datap->PositionL[child]);

			// Transform the child local matrix.
			childWorld *= invParentWorld;
			XMStoreFloat4x4(&_datap->Local[child], childWorld);

			// Transform the childs local pos.
			childWorldPos = XMVector3TransformCoord(childWorldPos, invParentWorld);
			XMStoreFloat3(&_datap->PositionL[child], childWorldPos);
	
			// Transform the childs local scale.
			XMVECTOR scale = XMLoadFloat3(&_datap->Scale[child]);
			scale = XMVectorDivide(scale, pscale);
			XMStoreFloat3(&_datap->Scale[child], scale);

			// Transform the childs local forward.
			XMVECTOR forward = XMLoadFloat3(&_datap->Forward[child]);
			forward = XMVector3Normalize(XMVector3TransformNormal(forward, invParentWorld));
			XMStoreFloat3(&_datap->Forward[child], forward);

			// Transform the childs local up.
			XMVECTOR Up = XMLoadFloat3(&_datap->Up[child]);
			Up = XMVector3Normalize(XMVector3TransformNormal(Up, invParentWorld));
			XMStoreFloat3(&_datap->Up[child], Up);

			// Transform the childs local right.
			XMStoreFloat3(&_datap->Right[child], XMVector3Cross(forward, Up));


		
			XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		
			float angleY = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultUp))) < 0.0001))
			{
				XMVECTOR projToXZ = XMVector3Normalize(forward - (defaultUp * XMVector3Dot(defaultUp, forward)));
				angleY = XMVectorGetX(XMVector3AngleBetweenNormals(projToXZ, defaultForward));
			}
			float angleX = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultForward))) < 0.0001))
			{
				XMVECTOR projToXY = XMVector3Normalize(forward - (defaultForward * XMVector3Dot(defaultForward, forward)));
				angleX = XM_PIDIV2 - XMVectorGetX(XMVector3AngleBetweenNormals(projToXY, defaultUp));
			}
			float angleZ = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(Up, defaultForward))) < 0.0001))
			{
				XMVECTOR projToXY = XMVector3Normalize(Up - (defaultForward * XMVector3Dot(defaultForward, Up)));
				float angleZ = XM_PIDIV2 - XMVectorGetX(XMVector3AngleBetweenNormals(projToXY, defaultRight));


			}

			XMStoreFloat3(&_datap->Rotation[child], XMVectorSet(angleX, angleY, angleZ, 0.0f));
			
		}
		const void TransformManager::_TransformFromParentLocal(uint32_t parent, uint32_t child)
		{
			using namespace DirectX;
			XMMATRIX parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
			XMMATRIX localWorld = XMLoadFloat4x4(&_datap->Local[child]);

			// Transform the childs local matrix.
			localWorld *= parentWorld;
			XMStoreFloat4x4(&_datap->Local[child], localWorld);

			// Transform the childs local pos.
			XMVECTOR childLocalPos = XMLoadFloat3(&_datap->PositionL[child]);
			childLocalPos = XMVector3TransformCoord(childLocalPos, parentWorld);
			XMStoreFloat3(&_datap->PositionL[child], childLocalPos);

			// Transform the childs local scale.
			XMVECTOR scale = XMLoadFloat3(&_datap->Scale[child]);
			do
			{
				scale *= XMLoadFloat3(&_datap->Scale[parent]);
				parent = _datap->Parent[parent];
			} while (parent != -1);
			XMStoreFloat3(&_datap->Scale[child], scale);

			// Transform the childs local forward.
			XMVECTOR forward = XMLoadFloat3(&_datap->Forward[child]);
			forward = XMVector3Normalize(XMVector3TransformNormal(forward, parentWorld));
			XMStoreFloat3(&_datap->Forward[child], forward);

			// Transform the childs local up.
			XMVECTOR Up = XMLoadFloat3(&_datap->Up[child]);
			Up = XMVector3Normalize(XMVector3TransformNormal(Up, parentWorld));
			XMStoreFloat3(&_datap->Up[child], Up);

			// Transform the childs local right.
			XMStoreFloat3(&_datap->Right[child], XMVector3Cross(forward, Up));



			XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

			float angleY = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultUp))) < 0.0001))
			{
				XMVECTOR projToXZ = XMVector3Normalize(forward - (defaultUp * XMVector3Dot(defaultUp, forward)));
				angleY = XMVectorGetX(XMVector3AngleBetweenNormals(projToXZ, defaultForward));
			}
			float angleX = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(forward, defaultForward))) < 0.0001))
			{
				XMVECTOR projToXY = XMVector3Normalize(forward - (defaultForward * XMVector3Dot(defaultForward, forward)));
				angleX = XM_PIDIV2 - XMVectorGetX(XMVector3AngleBetweenNormals(projToXY, defaultUp));
			}
			float angleZ = 0.0f;
			if (!(XMVectorGetX(XMVector3Length(XMVector3AngleBetweenNormals(Up, defaultForward))) < 0.0001))
			{
				XMVECTOR projToXY = XMVector3Normalize(Up - (defaultForward * XMVector3Dot(defaultForward, Up)));
				float angleZ = XM_PIDIV2 - XMVectorGetX(XMVector3AngleBetweenNormals(projToXY, defaultRight));


			}

			XMStoreFloat3(&_datap->Rotation[child], XMVectorSet(angleX, angleY, angleZ, 0.0f));
		}

		const void TransformManager::_Transform(uint32_t index)
		{
			using namespace DirectX;
			// Load everything
			XMVECTOR localPos = XMLoadFloat3(&_datap->PositionL[index]);
			XMVECTOR localRot = XMLoadFloat3(&_datap->Rotation[index]);
			XMVECTOR localScale = XMLoadFloat3(&_datap->Scale[index]);
			XMMATRIX parentWorld;

			// Calc matrices.
			XMMATRIX localWorld = XMMatrixScalingFromVector(localScale);
			XMMATRIX rotma = XMMatrixRotationRollPitchYawFromVector(localRot);
			localWorld *= rotma;
			localWorld *= XMMatrixTranslationFromVector(localPos);

			// Calc the forward, up and right vectors.
			XMVECTOR forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

			forward = XMVector3TransformNormal(forward, localWorld);
			up = XMVector3TransformNormal(up, localWorld);

			forward = XMVector3Normalize(forward);
			up = XMVector3Normalize(up);

			XMVECTOR right = XMVector3Cross(up, forward);


			// Store the new values.
			XMStoreFloat4x4(&_datap->Local[index], localWorld);
			XMStoreFloat3(&_datap->Forward[index], forward);
			XMStoreFloat3(&_datap->Up[index], up);
			XMStoreFloat3(&_datap->Right[index], right);

			uint32_t parent = _datap->Parent[index];
			if (parent != -1)
			{
				// Entity has a parent, so take that into account.
				parentWorld = XMLoadFloat4x4(&_datap->World[parent]);
				localWorld *= parentWorld;
			}
			XMStoreFloat4x4(&_datap->World[index], localWorld);

			uint32_t child = _datap->FirstChild[index];
			
			if (child != -1)
			{	
				// Entity had children, update their transforms aswell. (We won't do this recursively for some performance boost)
				std::vector<XMMATRIX> parentWorldTemp;
				parentWorldTemp.reserve(10);
				std::vector<uint32_t> childTemp;
				childTemp.reserve(10);

				parentWorld = localWorld;
				again:
				while (child != -1)
				{
					localPos = XMLoadFloat3(&_datap->PositionL[index]);
					localRot = XMLoadFloat3(&_datap->Rotation[index]);
					localScale = XMLoadFloat3(&_datap->Scale[index]);

					localWorld = XMMatrixScalingFromVector(localScale);
					localWorld *= XMMatrixRotationRollPitchYawFromVector(localRot);
					localWorld *= XMMatrixTranslationFromVector(localPos);
					
					// Calc the forward, up and right vectors.
					forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
					up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

					forward = XMVector3TransformNormal(forward, localWorld);
					up = XMVector3TransformNormal(up, localWorld);

					forward = XMVector3Normalize(forward);
					up = XMVector3Normalize(up);

					right = XMVector3Cross(up, forward);

					// Store the new values.
					XMStoreFloat4x4(&_datap->Local[index], localWorld);
					XMStoreFloat3(&_datap->Forward[index], forward);
					XMStoreFloat3(&_datap->Up[index], up);
					XMStoreFloat3(&_datap->Right[index], right);

					localWorld *= parentWorld;

					XMStoreFloat4x4(&_datap->World[index], localWorld);

					uint32_t c2 = _datap->FirstChild[child];
					if (c2 != -1)
					{
						parentWorldTemp.push_back(parentWorld);
						childTemp.push_back(_datap->NextSibling[child]);

						parentWorld = localWorld;
						child = c2;
					}
					else
					{
						child = _datap->NextSibling[child];
						
					}
					
				} 

				if (parentWorldTemp.size() > 0)
				{
					parentWorld = parentWorldTemp.back();
					parentWorldTemp.pop_back();

					child = childTemp.back();
					childTemp.pop_back();
					goto again;
				}
			}


				
		}
	}
}