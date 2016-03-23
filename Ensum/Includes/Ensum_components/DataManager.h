#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_
#pragma once
#include "Ensum_components\Manager.h"
#include "ensumstring.h"
#include "Ensum_utils\Exception.h"
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
		class ENSUM_COMPONENTS_EXPORT DataManager : public Manager
		{
		public:
			DataManager(EntityManager& entManager);
			~DataManager();

		protected:
			/** Allocate more memory for scenedata
			*
			*/
			const void _Allocate(uint32_t size);
			/** Delete an entry in the memory block.
			* The deleted entry is replaced by the last in the block.
			*/
			const void _Destroy(uint32_t index);

		public:
			enum class DataType : uint32_t
			{
				BOOL, FLOAT, STRING
			};


			struct ValueI
			{
				DataType type;

					bool b;
					float f;
					string str;
				//	std::vector<float> arr;

				
				ValueI(const ValueI& other) : type(other.type), b(other.b), f(other.f), str(other.str) {};
				ValueI(bool v) : b(v), type(DataType::BOOL) {};
				ValueI(float v) : f(v), type(DataType::FLOAT) {};
				ValueI(const char* s) : str(s), type(DataType::STRING) {};
				//ValueI(std::vector<float>& a) : arr(a), type(DataType::ARRAY) {};

				ValueI& operator=(const bool v) { b = v; type = DataType::BOOL; return *this; }
				ValueI& operator=(const float v) { f = v; type = DataType::FLOAT;  return *this; }
				ValueI& operator=(const string& s) { str = s; type = DataType::STRING; return *this; }
				//ValueI& operator=(std::vector<float>& a) { arr = a; type = DataType::ARRAY;  return *this; }

				~ValueI() {};
			};
		private:
			struct Data
			{
				uint16_t offset;
				uint16_t size;
			};
			struct Value
			{
				union
				{
					bool b;
					float f;
					Data data; // str
				};
			};

			struct EntryHeader
			{
				uint8_t capacity = 0;
				uint8_t entryCount = 0;
				uint16_t entrySize = sizeof(uint32_t) + sizeof(DataType) + sizeof(Value);

				uint32_t* keys;
				DataType* type;
				Value* value;	
			};

			struct Value_Buffer
			{
				uint32_t size = 0;
			};

			struct DataBuffer
			{
				void* data;
				uint32_t capacity;		

				EntryHeader header;
				Value_Buffer v_buffer;
				DataBuffer() : capacity(0), data(nullptr)
				{
					AllocateAndResizeHeader(header.entrySize * 50);
				}
				~DataBuffer()
				{
					free(data);
				}
				const void Allocate(uint32_t size)
				{
					if (size <= capacity) Exception("Buffer should only grow.");

					void* new_data = malloc(size);

					memcpy(new_data, data, header.capacity*header.entrySize);
					memcpy((void*)((uint32_t)new_data + size - v_buffer.size), (void*)((uint32_t)data + capacity - v_buffer.size), v_buffer.size);

					header.keys = (uint32_t*)new_data;
					header.type = (DataType*)(header.keys + sizeof(uint32_t)*header.capacity);
					header.value = (Value*)(header.type + sizeof(DataType)*header.capacity);
	
				


					free(data);
					data = new_data;

					capacity = size;
				}
				const void AllocateAndResizeHeader(uint32_t size)
				{
					if (size <= capacity) Exception("Buffer should only grow.");

					void* new_data = malloc(size);

					if ((((uint16_t)header.capacity) * 2 + 5) > ((uint16_t)~0 >> 8))
						header.capacity = ~0;
					else
						header.capacity += header.capacity + 5;

					uint32_t keyPos = (uint32_t)new_data;
					uint32_t typePos = keyPos + sizeof(uint32_t)*header.capacity;
					uint32_t valuePos = typePos + sizeof(DataType)*header.capacity;

					memcpy((void*)valuePos, header.value, sizeof(Value)*header.entryCount);
					memcpy((void*)typePos, header.type, sizeof(DataType)*header.entryCount);
					memcpy((void*)keyPos, header.keys, sizeof(uint32_t)*header.entryCount);
					
					header.value = (Value*)valuePos;
					header.type = (DataType*)typePos;
					header.keys = (uint32_t*)keyPos;

					memcpy((void*)((uint32_t)new_data + size - v_buffer.size), (void*)((uint32_t)data + capacity - v_buffer.size), v_buffer.size);

					free(data);
					data = new_data;

					capacity = size;
				}

				const void HeaderResize()
				{
					if ((((uint16_t)header.capacity) * 2 + 5) > ((uint16_t)~0 >> 8))
						header.capacity = ~0;
					else
						header.capacity += header.capacity + 5;

					uint32_t typePos = (uint32_t)header.keys + sizeof(uint32_t)*header.capacity;
					uint32_t valuePos = typePos + sizeof(DataType)*header.capacity;

					memcpy((void*)valuePos, header.value, sizeof(Value)*header.entryCount);
					memcpy((void*)typePos, header.type, sizeof(DataType)*header.entryCount);

					header.value = (Value*)valuePos;
					header.type = (DataType*)typePos;

				}

				const void AddBoolValue(const string& key, const bool val)
				{
					uint16_t newheaderSize = (header.entryCount + 1)*header.entrySize;
					uint32_t totalNewSize = newheaderSize + v_buffer.size;

					if (header.entryCount + 1 > header.capacity)
					{
						// Header alloc
						if (totalNewSize + (header.capacity + 5)*header.entrySize > capacity)
							AllocateAndResizeHeader(totalNewSize * 2 + 50);
						else
							HeaderResize();

					}
					else if (totalNewSize > capacity)
						Allocate(totalNewSize * 2 + 50);

					const uint32_t keyh = key.GetHash();

					header.keys[header.entryCount] = keyh;
					header.type[header.entryCount] = DataType::BOOL;
					header.value[header.entryCount].b = val;

					header.entryCount++;
				}
				const void AddFloatValue(const string& key, const float val)
				{
					uint16_t newheaderSize = (header.entryCount + 1)*header.entrySize;
					uint32_t totalNewSize = newheaderSize + v_buffer.size;
					
					if (header.entryCount + 1 > header.capacity)
					{
						// Header alloc
						if (totalNewSize + (header.capacity + 5)*header.entrySize > capacity)
							AllocateAndResizeHeader(totalNewSize * 2 + 50);
						else
							HeaderResize();

					}
					else if (totalNewSize > capacity)
						Allocate(totalNewSize * 2 + 50);

					const uint32_t keyh = key.GetHash();

					header.keys[header.entryCount] = keyh;
					header.type[header.entryCount] = DataType::FLOAT;
					header.value[header.entryCount].f = val;

					header.entryCount++;
				}
				const void AddStringValue(const string& key, const string& val)
				{
					uint16_t newheaderSize = (header.entryCount + 1)*header.entrySize;
					uint32_t totalNewSize = newheaderSize + v_buffer.size;

					// String lookahead
					totalNewSize += val.size()*sizeof(char);


					if (header.entryCount + 1 > header.capacity)
					{
						// Header alloc
						if (totalNewSize + (header.capacity + 5)*header.entrySize > capacity)
							AllocateAndResizeHeader(totalNewSize * 2 + 50);
						else
							HeaderResize();

					}
					else if (totalNewSize > capacity)
						Allocate(totalNewSize * 2 + 50);

					const uint32_t keyh = key.GetHash();

					header.keys[header.entryCount] = keyh;
					header.type[header.entryCount] = DataType::STRING;

					Data& d = header.value[header.entryCount].data;
					d.size = val.size()*sizeof(char);
					v_buffer.size += d.size;
					d.offset = v_buffer.size;
					memcpy((void*)((uint32_t)data + capacity - d.offset), val, d.size);

					header.entryCount++;
				}


				const void SetBoolValue(const string& key, const bool val)
				{
					const uint32_t keyh = key.GetHash();
					for (uint32_t i = 0; i < header.entryCount; i++)
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
					Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", 0, key);
				}
				const void SetFloatValue(const string& key, const float val)
				{
					const uint32_t keyh = key.GetHash();
					for (uint32_t i = 0; i < header.entryCount; i++)
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
					Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", 0, key);
				}
				const void SetStringValue(const string& key, const string& val)
				{
					const uint32_t keyh = key.GetHash();
					for (uint32_t i = 0; i < header.entryCount; i++)
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
							d.size = val.size()*sizeof(char);
							if (d.size > prevSize)
							{
								v_buffer.size += d.size;
								d.offset = v_buffer.size;
							}
							else
								d.offset -= prevSize - d.size;
							memcpy((void*)((uint32_t)data + capacity - d.offset), val, d.size);
						}
					}

					Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", 0, key);
				}

				bool GetBoolValue(const string& key)
				{
					const uint32_t keyh = key.GetHash();
					for (uint32_t i = 0; i < header.entryCount; i++)
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
					Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", 0, key);
					return false;
				}	
				float GetFloatValue(const string& key)
				{
					const uint32_t keyh = key.GetHash();
					for (uint32_t i = 0; i < header.entryCount; i++)
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
					Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", 0, key);
					return 0.0f;
				}
				string GetStringValue(const string& key)
				{
					const uint32_t keyh = key.GetHash();
					for (uint32_t i = 0; i < header.entryCount; i++)
					{
						if (header.keys[i] == keyh)
						{
							if (header.type[i] != DataType::STRING)
							{
								Utils::ConsoleLog::DumpToConsole("Tried to get string value from entity data, but entry was not of type string.");
								return false;
							}
							Data& d = header.value[i].data;
							string s((char*)((uint32_t)data + capacity - d.offset));
							s.Resize(d.size);

							return s;
						}
					}

					Utils::ConsoleLog::DumpToConsole("Entity data entry key could not be found. Entity: %d Key: %s.", 0, key);
					return "";
				}
			};

			


		};



	}
}
#endif