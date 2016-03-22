#ifndef _MANAGER_H_
#define _MANAGER_H_

#pragma once
#include "dll_export.h"
#include "Data_Meta.h"
#include <random>
#include <unordered_map>
#include "Entity.h"

namespace Ensum
{
	namespace Components
	{
		class ENSUM_COMPONENTS_EXPORT Manager
		{
		protected:
			struct ManagerMetaData : public MetaData
			{
				Entity* entity;
			};

			Manager(ManagerMetaData* data) :
				_entityToIndex(new std::unordered_map<Entity, uint32_t, EntityHasher>), 
				_data(data) 
			{
			
			};
		public:
			virtual ~Manager() 
			{ 
				delete _entityToIndex; _entityToIndex = nullptr;
				std::free(_data->buffer);
				delete _data; _data = nullptr;
		
			};

			/** Do the framework for all active scenes.
			*
			*/
			virtual const void Frame() = 0;
		protected:
			/** Allocate more memory for scenedata
			*
			*/
			virtual const void _Allocate(uint32_t size) = 0;
			/** Delete an entry in the memory block.
			* The deleted entry is replaced by the last in the block.
			*/
			virtual const void _Destroy(uint32_t index) = 0;
			/** Looks for destroyed entities and deletes them from the data block.
			*
			*/
			virtual const void _GC() = 0;
		protected:
			std::default_random_engine _generator;
			std::unordered_map<Entity, uint32_t, EntityHasher>* _entityToIndex;
			ManagerMetaData* _data;
		};
	}
}
#endif