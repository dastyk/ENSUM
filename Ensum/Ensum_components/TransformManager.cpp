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

			if (_data->used + 1 >= _data->allocated)
				_Allocate(_data->allocated * 2 + 10);

			if (!_entityManager.Alive(entity))
			{
				Utils::ConsoleLog::DumpToConsole("Tried to bind transformcomponent to a dead entity. Entity: %d", entity.ID);
				return;
			}

			

			Utils::ConsoleLog::DumpToConsole("Creating Datacomponent for Entity: %d", entity.ID);

			uint32_t index = (*_entityToIndex)[entity] = static_cast<uint32_t>(_entityToIndex->size());
			_datap->entity[index] = entity;

			XMStoreFloat4x4(&_datap->Local[index], DirectX::XMMatrixIdentity());
			XMStoreFloat4x4(&_datap->World[index], DirectX::XMMatrixIdentity());

			_datap->Parent[index] = -1;
			_datap->FirstChild[index] = -1;
			_datap->PrevSibling[index] = -1;
			_datap->NextSibling[index] = -1;

		/*	_datap->PositionL[index] = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			_datap->PositionW[index] = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			_datap->Rotation[index] = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			_datap->Scale[index] = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

			_datap->Forward[index] = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			_datap->Up[index] = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			_datap->Right[index] = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);*/

			_datap->restrictMovements[index] = false;
			_datap->used++;
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
		const void TransformManager::_Allocate(uint32_t size)
		{
			if (size <= _datap->allocated) Exception("Alloc should only increase.");

			TransformComponents* new_data = new TransformComponents;
			size_t bytes = static_cast<size_t>(size * (sizeof(Entity) + sizeof(uint32_t) * 4 + sizeof(DirectX::XMFLOAT4X4) * 2 + sizeof(DirectX::XMFLOAT3) * 7 + sizeof(bool)));
			new_data->buffer = operator new(bytes);
			new_data->used = _datap->used;
			new_data->allocated = size;

			new_data->entity = (Entity*)(new_data->buffer);

			new_data->Parent = (uint32_t*)(new_data->entity + static_cast<size_t>(size));
			new_data->FirstChild = (uint32_t*)(new_data->FirstChild + static_cast<size_t>(size));
			new_data->PrevSibling = (uint32_t*)(new_data->PrevSibling + static_cast<size_t>(size));
			new_data->NextSibling = (uint32_t*)(new_data->NextSibling + static_cast<size_t>(size));

			new_data->Local = (DirectX::XMFLOAT4X4*)(new_data->NextSibling + static_cast<size_t>(size));
			new_data->World = (DirectX::XMFLOAT4X4*)(new_data->Local + static_cast<size_t>(size));

			//new_data->PositionL = (DirectX::XMFLOAT3*)(new_data->World + static_cast<size_t>(size));
			//new_data->PositionW = (DirectX::XMFLOAT3*)(new_data->PositionL + static_cast<size_t>(size));
			//new_data->Rotation = (DirectX::XMFLOAT3*)(new_data->PositionW + static_cast<size_t>(size));
			//new_data->Scale = (DirectX::XMFLOAT3*)(new_data->Rotation + static_cast<size_t>(size));

			//new_data->Forward = (DirectX::XMFLOAT3*)(new_data->Scale + static_cast<size_t>(size));
			//new_data->Up = (DirectX::XMFLOAT3*)(new_data->Forward + static_cast<size_t>(size));
			//new_data->Right = (DirectX::XMFLOAT3*)(new_data->Up + static_cast<size_t>(size));

			new_data->restrictMovements = (bool*)(new_data->World + static_cast<size_t>(size));

			memcpy(new_data->entity, _datap->entity, _datap->used * sizeof(Entity));

			memcpy(new_data->Parent, _datap->Parent, _datap->used * sizeof(uint32_t));
			memcpy(new_data->FirstChild, _datap->FirstChild, _datap->used * sizeof(uint32_t));
			memcpy(new_data->PrevSibling, _datap->PrevSibling, _datap->used * sizeof(uint32_t));
			memcpy(new_data->NextSibling, _datap->NextSibling, _datap->used * sizeof(uint32_t));

			memcpy(new_data->Local, _datap->Local, _datap->used * sizeof(DirectX::XMFLOAT4X4));
			memcpy(new_data->World, _datap->World, _datap->used * sizeof(DirectX::XMFLOAT4X4));
			
		/*	memcpy(new_data->PositionL, _datap->PositionL, _datap->used * sizeof(DirectX::XMFLOAT3));
			memcpy(new_data->PositionW, _datap->PositionW, _datap->used * sizeof(DirectX::XMFLOAT3));
			memcpy(new_data->Rotation, _datap->Rotation, _datap->used * sizeof(DirectX::XMFLOAT3));
			memcpy(new_data->Scale, _datap->Scale, _datap->used * sizeof(DirectX::XMFLOAT3));

			memcpy(new_data->Forward, _datap->Forward, _datap->used * sizeof(DirectX::XMFLOAT3));
			memcpy(new_data->Up, _datap->Up, _datap->used * sizeof(DirectX::XMFLOAT3));
			memcpy(new_data->Right, _datap->Right, _datap->used * sizeof(DirectX::XMFLOAT3));*/

			memcpy(new_data->restrictMovements, _datap->restrictMovements, _datap->used * sizeof(bool));

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

		/*	_datap->PositionL[index] = _datap->PositionL[last];
			_datap->PositionW[index] = _datap->PositionW[last];
			_datap->Rotation[index] = _datap->Rotation[last];
			_datap->Scale[index] = _datap->Scale[last];

			_datap->Forward[index] = _datap->Forward[last];
			_datap->Up[index] = _datap->Up[last];
			_datap->Right[index] = _datap->Right[last];
*/
			_datap->restrictMovements[index] = _datap->restrictMovements[last];

			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_datap->used--;
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

			_datap->Parent[child] == -1;
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
		const void TransformManager::_TransformToParentLocal(uint32_t parent, uint32_t child)
		{
			DirectX::XMMATRIX parentWorld = DirectX::XMLoadFloat4x4(&_datap->World[parent]);
			DirectX::XMMATRIX invParentWorld = DirectX::XMMatrixInverse(nullptr, parentWorld);

			DirectX::XMMATRIX childWorld = DirectX::XMLoadFloat4x4(&_datap->World[child]);

			childWorld *= invParentWorld;
			DirectX::XMStoreFloat4x4(&_datap->Local[child], childWorld);

			//DirectX::XMVECTOR childWorldPos = DirectX::XMLoadFloat3(&_datap->PositionW[child]);
			//childWorldPos = DirectX::XMVector3Transform(childWorldPos, invParentWorld);
			//DirectX::XMStoreFloat3(&_datap->PositionL[child], childWorldPos);

			// Hmm, wonder what to do with rotation and scale....

		}
		const void TransformManager::_TransformFromParentLocal(uint32_t parent, uint32_t child)
		{
			DirectX::XMMATRIX parentWorld = DirectX::XMLoadFloat4x4(&_datap->World[parent]);

			DirectX::XMMATRIX localWorld = DirectX::XMLoadFloat4x4(&_datap->Local[child]);

			localWorld *= parentWorld;
			DirectX::XMStoreFloat4x4(&_datap->Local[child], localWorld);

		/*	DirectX::XMVECTOR childLocalPos = DirectX::XMLoadFloat3(&_datap->PositionL[child]);
			childLocalPos = DirectX::XMVector3Transform(childLocalPos, parentWorld);
			DirectX::XMStoreFloat3(&_datap->PositionL[child], childLocalPos);
*/
			// Hmm, wonder what to do with rotation and scale....
		}
	}
}