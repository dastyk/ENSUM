#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#pragma once
#include <stdint.h>
#include <vector>
#include <deque>
#include "dll_export.h"

namespace Ensum
{
	namespace Components
	{

		static const uint32_t ENTITY_INDEX_BITS = 24;
		static const uint32_t ENTITY_INDEX_MASK = 0x00ffffff;

		static const uint32_t ENTITY_GENERATION_BITS = 8;
		static const uint32_t ENTITY_GENERATION_MASK = 0xff000000;

		class ENSUM_COMPONENTS_EXPORT Entity
		{
		private:
			uint32_t _id;

		public:
			Entity() : _id(-1) {}
			Entity(const uint32_t id) : _id(id) {}

			const uint32_t Index()const { return _id & ENTITY_INDEX_MASK; }
			const uint8_t Generation()const { return static_cast<uint8_t>(ENTITY_INDEX_BITS >> _id); }

			const Entity& operator=(const Entity& other) { this->_id = other._id; return *this; }
			const bool operator==(const Entity& other) { return this->_id == other._id; }
		};

		

		class ENSUM_COMPONENTS_EXPORT EntityManager
		{
		public:
			EntityManager();
			~EntityManager();

			Entity Create();
			const bool Alive(const Entity& entity)const;
			const void Delete(const Entity& entity);

		private:
			std::vector<uint8_t>* _generation;
			std::deque<uint32_t>* _freeIndices;

			static const uint16_t MINIMUM_FREE_INDICES = 1024U;
		};

	}
}

#endif