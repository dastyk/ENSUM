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

			Manager(ManagerMetaData* data, EntityManager& entManager) :
				_entityToIndex(new std::unordered_map<Entity, uint32_t, EntityHasher>), 
				_data(data),
				_entityManager(entManager)
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
			virtual const void Frame()
			{
				_GC();
			}
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
			virtual const void _GC()
			{
				uint32_t alive_in_row = 0;
				while (_data->used > 0 && alive_in_row < 4U)
				{
					std::uniform_int_distribution<uint32_t> distribution(0U, _data->used - 1U);
					uint32_t i = distribution(_generator);
					if (_entityManager.Alive(_data->entity[i]))
					{
						alive_in_row++;
						continue;
					}
					alive_in_row = 0;
					_Destroy(i);
				}
			};
		protected:
			std::default_random_engine _generator;
			std::unordered_map<Entity, uint32_t, EntityHasher>* _entityToIndex;
			ManagerMetaData* _data;
			EntityManager& _entityManager;
		};
	}
}
#endif