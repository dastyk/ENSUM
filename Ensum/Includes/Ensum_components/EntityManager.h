#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#pragma once
#include <stdint.h>
#include <vector>
#include <deque>
#include "dll_export.h"
#include "Ensum_components\Entity.h"

namespace Ensum
{
	namespace Components
	{
		/** Manages all entities in all scenes.
		*
		*/
		class ENSUM_COMPONENTS_EXPORT EntityManager
		{
		public:
			EntityManager();
			~EntityManager();

			/** Creates an entity, and returns it.
			*
			*/
			Entity Create();
			/** Determines whether or not the entity is alive.
			*
			*/
			const bool Alive(const Entity& entity)const;
			/** Deletes an alive entity.
			*
			*/
			const void Delete(const Entity& entity);

		private:
			std::vector<uint8_t>* _generation;
			std::deque<uint32_t>* _freeIndices;

			static const uint16_t MINIMUM_FREE_INDICES = 1024U;
		};

	}
}

#endif