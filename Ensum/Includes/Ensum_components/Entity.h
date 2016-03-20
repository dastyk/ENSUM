#ifndef _ENTITY_H_
#define _ENTITY_H_

#pragma once
#include "dll_export.h"
#include <stdint.h>

namespace Ensum
{
	namespace Components
	{
		static const uint32_t ENTITY_INDEX_BITS = 24;
		static const uint32_t ENTITY_INDEX_MASK = 0x00ffffff;

		static const uint32_t ENTITY_GENERATION_BITS = 8;
		static const uint32_t ENTITY_GENERATION_MASK = 0xff000000;

		/** Entity is essentially a id, divided into generation and index.
		*
		*/
		class ENSUM_COMPONENTS_EXPORT Entity
		{
		public:
			Entity() : _id(-1) {}
			Entity(const uint32_t id) : _id(id) {}

			/** Returns the index part of the entity ID. 
			*  
			*/
			const uint32_t Index()const { return _id & ENTITY_INDEX_MASK; }
			/** Returns the genration part of the entity ID.
			*
			*/
			const uint8_t Generation()const { return static_cast<uint8_t>(_id >> ENTITY_INDEX_BITS); }

			const Entity& operator=(const Entity& other) { this->_id = other._id; return *this; }
			const bool operator==(const Entity& other) { return this->_id == other._id; }

		private:
			uint32_t _id; /*!< The actual id of the entity, the last 8 bits are the current generation of the entity, and the first 24 are the index. */

		};
	}
}
#endif