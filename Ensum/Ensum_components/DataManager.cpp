#include "Ensum_components\DataManager.h"
#include "Safe_Delete.h"

namespace Ensum
{
	namespace Components
	{
		DataManager::DataManager(EntityManager & entManager) :
			Manager(new EntityData, entManager), 
			_datap(((EntityData*)(_data)))
		{
			_Allocate(10);
		}

		DataManager::~DataManager()
		{
			for (auto& e : (*_entityToIndex))
			{
				SAFE_DELETE(_datap->dataBuff[e.second]);
			}
		}
		const void DataManager::CreateData(const Entity & entity)
		{
			if (!_entityManager.Alive(entity))
			{
				Utils::ConsoleLog::DumpToConsole("Tried to bind datacomponent to a dead entity. Entity: %d", entity.ID);
				return;
			}

			if (_datap->used >= _datap->allocated)
				_Allocate(static_cast<uint32_t>(_datap->allocated * 1.5f) + 10);

			Utils::ConsoleLog::DumpToConsole("Creating Datacomponent for Entity: %d", entity.ID);

			uint32_t index = (*_entityToIndex)[entity] = static_cast<uint32_t>(_entityToIndex->size());
			_datap->entity[index] = entity;
			_datap->dataBuff[index] = new DataBuffer(entity);
			_datap->used++;
		}
		const void DataManager::AddBoolValue(const Entity & entity, const string & key, const bool val)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				size_t newheaderSize = static_cast<size_t>((header.entryCount + 1)*header.entrySize);
				size_t totalNewSize = newheaderSize + buffer.v_buffer.size;

				if (header.entryCount + 1 > header.capacity)
				{
					// Header alloc
					if (totalNewSize + static_cast<size_t>((header.capacity + 5)*header.entrySize) > buffer.capacity)
						buffer.AllocateAndResizeHeader(totalNewSize * 2 + 50);
					else
						buffer.HeaderResize();

				}
				else if (totalNewSize > buffer.capacity)
					buffer.Allocate(totalNewSize * 2 + 50);

				const uint32_t keyh = key.GetHash();

				header.keys[header.entryCount] = keyh;
				header.type[header.entryCount] = DataType::BOOL;
				header.value[header.entryCount].b = val;

				header.entryCount++;
			}
			
		}
		const void DataManager::AddFloatValue(const Entity & entity, const string & key, const float val)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				size_t newheaderSize = static_cast<size_t>((header.entryCount + 1)*header.entrySize);
				size_t totalNewSize = newheaderSize + buffer.v_buffer.size;

				if (header.entryCount + 1 > header.capacity)
				{
					// Header alloc
					if (totalNewSize + static_cast<size_t>((header.capacity + 5)*header.entrySize) > buffer.capacity)
						buffer.AllocateAndResizeHeader(totalNewSize * 2 + 50);
					else
						buffer.HeaderResize();

				}
				else if (totalNewSize > buffer.capacity)
					buffer.Allocate(totalNewSize * 2 + 50);

				const uint32_t keyh = key.GetHash();

				header.keys[header.entryCount] = keyh;
				header.type[header.entryCount] = DataType::FLOAT;
				header.value[header.entryCount].f = val;

				header.entryCount++;
			}
		}
		const void DataManager::AddStringValue(const Entity & entity, const string & key, const string & val)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				size_t newheaderSize = static_cast<size_t>((header.entryCount + 1)*header.entrySize);
				size_t totalNewSize = newheaderSize + buffer.v_buffer.size;

				if (header.entryCount + 1 > header.capacity)
				{
					// Header alloc
					if (totalNewSize + static_cast<size_t>((header.capacity + 5)*header.entrySize) > buffer.capacity)
						buffer.AllocateAndResizeHeader(totalNewSize * 2 + 50);
					else
						buffer.HeaderResize();

				}
				else if (totalNewSize > buffer.capacity)
					buffer.Allocate(totalNewSize * 2 + 50);

				const uint32_t keyh = key.GetHash();

				header.keys[header.entryCount] = keyh;
				header.type[header.entryCount] = DataType::STRING;

				Data& d = header.value[header.entryCount].data;
				d.size = static_cast<uint16_t>(val.size()*sizeof(char));
				buffer.v_buffer.size += static_cast<size_t>(d.size);
				d.offset = static_cast<uint16_t>(buffer.v_buffer.size);
				memcpy((void*)((size_t)buffer.data + buffer.capacity - static_cast<size_t>(d.offset)), val, d.size);

				header.entryCount++;
			}
		}
		const void DataManager::SetBoolValue(const Entity & entity, const string & key, const bool val)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				const uint32_t keyh = key.GetHash();
				for (uint8_t i = 0; i < header.entryCount; i++)
				{
					if (header.keys[i] == keyh)
					{
						if (header.type[i] != DataType::BOOL)
						{
							Utils::ConsoleLog::DumpToConsole("Tried to set bool value from entity data, but entry was not of type bool.");
							return;
						}
						header.value[i].b = val;
					}
				}
				Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
			}
		}
		const void DataManager::SetFloatValue(const Entity & entity, const string & key, const float val)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				const uint32_t keyh = key.GetHash();
				for (uint8_t i = 0; i < header.entryCount; i++)
				{
					if (header.keys[i] == keyh)
					{
						if (header.type[i] != DataType::FLOAT)
						{
							Utils::ConsoleLog::DumpToConsole("Tried to set float value from entity data, but entry was not of type float.");
							return;
						}
						header.value[i].f = val;
					}
				}
				Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
			}
		}
		const void DataManager::SetStringValue(const Entity & entity, const string & key, const string & val)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				auto& v_buffer = buffer.v_buffer;
				const uint32_t keyh = key.GetHash();
				for (uint8_t i = 0; i < header.entryCount; i++)
				{
					if (header.keys[i] == keyh)
					{
						if (header.type[i] != DataType::STRING)
						{
							Utils::ConsoleLog::DumpToConsole("Tried to get string value from entity data, but entry was not of type string.");
							return;
						}

						Data& d = header.value[i].data;
						uint16_t prevSize = d.size;
						d.size = static_cast<uint16_t>(val.size()*sizeof(char));
						if (d.size > prevSize)
						{
							v_buffer.size += static_cast<size_t>(d.size);
							d.offset = static_cast<uint16_t>(v_buffer.size);
						}
						else
							d.offset -= prevSize - d.size;
						memcpy((void*)((size_t)buffer.data + buffer.capacity - static_cast<size_t>(d.offset)), val, d.size);
					}
				}

				Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
			}
		}
		bool DataManager::GetBoolValue(const Entity & entity, const string & key)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				const uint32_t keyh = key.GetHash();
				for (uint8_t i = 0; i < header.entryCount; i++)
				{
					if (header.keys[i] == keyh)
					{
						if (header.type[i] != DataType::BOOL)
						{
							Utils::ConsoleLog::DumpToConsole("Tried to get bool value from entity data, but entry was not of type bool.");
							return false;
						}
						return header.value[i].b;
					}
				}
				Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
			}
			return false;
		}
		float DataManager::GetFloatValue(const Entity & entity, const string & key)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				const uint32_t keyh = key.GetHash();
				for (uint8_t i = 0; i < header.entryCount; i++)
				{
					if (header.keys[i] == keyh)
					{
						if (header.type[i] != DataType::FLOAT)
						{
							Utils::ConsoleLog::DumpToConsole("Tried to get float value from entity data, but entry was not of type float.");
							return false;
						}
						return header.value[i].f;
					}
				}
				Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
			}
			return 0.0f;
		}
		string DataManager::GetStringValue(const Entity & entity, const string & key)
		{
			auto& find = _entityToIndex->find(entity);
			if (find != _entityToIndex->end())
			{
				auto& buffer = *_datap->dataBuff[find->second];
				auto& header = buffer.header;
				const uint32_t keyh = key.GetHash();
				for (uint8_t i = 0; i < header.entryCount; i++)
				{
					if (header.keys[i] == keyh)
					{
						if (header.type[i] != DataType::STRING)
						{
							Utils::ConsoleLog::DumpToConsole("Tried to get string value from entity data, but entry was not of type string.");
							return false;
						}
						Data& d = header.value[i].data;
						string s((char*)((size_t)buffer.data + buffer.capacity - static_cast<size_t>(d.offset)));
						s.Resize(d.size);

						return s;
					}
				}

				Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", entity.ID, key.c_str());
			}
			return "";
		}
		const void DataManager::_Allocate(uint32_t size)
		{
			if (size <= _datap->allocated) Exception("Alloc should only increase.");

			EntityData* new_data = new EntityData;
			size_t bytes = static_cast<size_t>(size * (sizeof(Entity) + sizeof(DataBuffer*)));
			new_data->buffer = operator new(bytes);
			new_data->used = _datap->used;
			new_data->allocated = size;

			new_data->entity = (Entity*)(new_data->buffer);
			new_data->dataBuff = (DataBuffer**)(new_data->entity + static_cast<size_t>(size));
		
			memcpy(new_data->entity, _datap->entity, _datap->used * sizeof(Entity));
			memcpy(new_data->dataBuff, _datap->dataBuff, _datap->used * sizeof(DataBuffer*));
	

			operator delete(_datap->buffer);
			delete _data;
			_data = new_data;
			_datap = (EntityData*)_data;
		}
		const void DataManager::_Destroy(uint32_t index)
		{
			uint32_t last = _datap->used - 1;
			const Entity& e = _datap->entity[index];
			const Entity& last_e = _datap->entity[last];

			_datap->entity[index] = _datap->entity[last];
			SAFE_DELETE(_datap->dataBuff[index]);
			_datap->dataBuff[index] = _datap->dataBuff[last];

			(*_entityToIndex)[last_e] = index;
			_entityToIndex->erase(e);

			_datap->used--;
		}













		DataManager::DataBuffer::DataBuffer(const Entity & ent) : capacity(0), data(nullptr)
		{
			AllocateAndResizeHeader(header.entrySize * 50);
		}
		DataManager::DataBuffer::~DataBuffer()
		{
			operator delete(data);
		}

		const void DataManager::DataBuffer::Allocate(size_t size)
		{
			if (size <= capacity) Exception("Buffer should only grow.");

			void* new_data = operator new(size);

			memcpy(new_data, data, header.capacity*header.entrySize);
			memcpy((void*)((size_t)new_data + size - v_buffer.size), (void*)((size_t)data + capacity - v_buffer.size), v_buffer.size);

			header.keys = (uint32_t*)new_data;
			header.type = (DataType*)(header.keys + sizeof(uint32_t)*header.capacity);
			header.value = (Value*)(header.type + sizeof(DataType)*header.capacity);




			operator delete(data);
			data = new_data;

			capacity = size;
		}
		const void DataManager::DataBuffer::AllocateAndResizeHeader(size_t size)
		{
			if (size <= capacity) Exception("Buffer should only grow.");

			void* new_data = operator new(size);

			if ((((uint16_t)header.capacity) * 2 + 5) > ((uint16_t)~0 >> 8))
				header.capacity = ~0;
			else
				header.capacity += header.capacity + 5;

			size_t keyPos = (size_t)new_data;
			size_t typePos = keyPos + sizeof(size_t)*header.capacity;
			size_t valuePos = typePos + sizeof(size_t)*header.capacity;

			memcpy((void*)valuePos, header.value, sizeof(Value)*header.entryCount);
			memcpy((void*)typePos, header.type, sizeof(DataType)*header.entryCount);
			memcpy((void*)keyPos, header.keys, sizeof(uint32_t)*header.entryCount);

			header.value = (Value*)valuePos;
			header.type = (DataType*)typePos;
			header.keys = (uint32_t*)keyPos;

			memcpy((void*)((size_t)new_data + size - v_buffer.size), (void*)((size_t)data + capacity - v_buffer.size), v_buffer.size);

			operator delete(data);
			data = new_data;

			capacity = size;
		}
		const void DataManager::DataBuffer::HeaderResize()
		{
			if ((((uint16_t)header.capacity) * 2 + 5) > ((uint16_t)~0 >> 8))
				header.capacity = ~0;
			else
				header.capacity += header.capacity + 5;

			size_t typePos = (size_t)header.keys + sizeof(uint32_t)*header.capacity;
			size_t valuePos = typePos + sizeof(DataType)*header.capacity;

			memcpy((void*)valuePos, header.value, sizeof(Value)*header.entryCount);
			memcpy((void*)typePos, header.type, sizeof(DataType)*header.entryCount);

			header.value = (Value*)valuePos;
			header.type = (DataType*)typePos;

		}
	}
}