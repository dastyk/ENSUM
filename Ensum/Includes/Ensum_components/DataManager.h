#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_
#pragma once
#include "Ensum_components\Manager.h"
#include "ensumstring.h"
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
		class ENSUM_COMPONENTS_EXPORT DataManager : public Manager
		{
		private:
			/** enum class for the kinds of data that can be stored as an entry.
			*
			*/
			enum class DataType : uint8_t
			{
				BOOL, FLOAT, STRING
			};
			/** A data struct used for indexing the strings and other growing datatypes in the value_buffer.
			*
			*/
			struct Data
			{
				uint16_t offset;
				uint16_t size;
			};
			/** The value struct, this is where the data is stored.
			* The union clumps the memorylocation of all the members to the same position.
			* They share the same spot.
			*/
			struct Value
			{
				union
				{
					bool b;
					float f;
					Data data; // str
				};
			};
			/** The header struct, used for keeping track of the data entries a entity has.
			*
			*/
			struct EntryHeader
			{
				uint8_t capacity = 0;
				uint8_t entryCount = 0;
				uint8_t entrySize = sizeof(uint32_t) + sizeof(DataType) + sizeof(Value);

				uint32_t* keys;
				DataType* type;
				Value* value;
			};

			/** Points to the next free spot in the value_buffer.
			*
			*/
			struct Value_Buffer
			{
				size_t size = 0;
			};

			/** Struct for keeping track of the data entries a entity has been given.
			* The databuffer stores the header on the left side and the value_buffer on the right side.
			* When the buffers meet the size is increased.
			*/
			struct DataBuffer
			{
				void* data;
				size_t capacity;

				EntryHeader header;
				Value_Buffer v_buffer;

				DataBuffer(const Entity& ent);
				~DataBuffer();

				/** Grow the entry buffer.
				*
				*/
				const void Allocate(size_t size);
				/** Grow the entry buffer and copy the header immediately.
				*
				*/
				const void AllocateAndResizeHeader(size_t size);
				/** Grow the header.
				*
				*/
				const void HeaderResize();
			};

			/** The managers data struct.
			*
			*/
			struct EntityData : public ManagerMetaData
			{
				DataBuffer** dataBuff;
			};
			EntityData* _datap; /*!< A reference pointer to avoid having to cast the basic datapointer all the time. */

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