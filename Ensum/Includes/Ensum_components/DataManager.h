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
		private:
			enum class DataType : uint32_t
			{
				BOOL, FLOAT, STRING
			};
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

			/** Struct for keeping track of the data entries a entity has been given.
			*
			*/
			struct DataBuffer
			{
				void* data;
				uint32_t capacity;

				EntryHeader header;
				Value_Buffer v_buffer;

				DataBuffer(const Entity& ent);
				~DataBuffer();

				/** Grow the entry buffer.
				*
				*/
				const void Allocate(uint32_t size);
				/** Grow the entry buffer and copy the header immediately.
				*
				*/
				const void AllocateAndResizeHeader(uint32_t size);
				/** Grow the header.
				*
				*/
				const void HeaderResize();
			};


			struct EntityData : public ManagerMetaData
			{
				DataBuffer** dataBuff;
			};
			EntityData* _datap;

		public:
			DataManager(EntityManager& entManager);
			~DataManager();
			/** Create the data component for the entity.
			*
			*/
			const void CreateData(const Entity& entity);
			


			/** Add the bool value to the given entity and key.
			*
			*/
			const void AddBoolValue(const Entity& entity, const string& key, const bool val);
			/** Add the float value to the given entity and key.
			*
			*/
			const void AddFloatValue(const Entity& entity, const string& key, const float val);
			/** Add the string value to the given entity and key.
			*
			*/
			const void AddStringValue(const Entity& entity, const string& key, const string& val);



			/** Set the bool value to the given entity and key.
			*
			*/
			const void SetBoolValue(const Entity& entity, const string& key, const bool val);
			/** Set the float value to the given entity and key.
			*
			*/
			const void SetFloatValue(const Entity& entity, const string& key, const float val);
			/** Set the string value to the given entity and key.
			*
			*/
			const void SetStringValue(const Entity& entity, const string& key, const string& val);



			/** Get the bool value to the given entity and key.
			*
			*/
			bool GetBoolValue(const Entity& entity, const string& key);
			/** Get the float value to the given entity and key.
			*
			*/
			float GetFloatValue(const Entity& entity, const string& key);
			/** Get the string value to the given entity and key.
			*
			*/
			string GetStringValue(const Entity& entity, const string& key);


		private:
			/** Allocate more memory
			*
			*/
			const void _Allocate(uint32_t size);
			/** Delete an entry in the memory block.
			* The deleted entry is replaced by the last in the block.
			*/
			const void _Destroy(uint32_t index);
		};



	}
}
#endif