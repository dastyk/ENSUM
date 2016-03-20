#ifndef _SCENE_H_
#define _SCENE_H_

#pragma once
#include "Ensum_components\EntityManager.h"
namespace Ensum
{
	namespace Components
	{
		/** The abstract scene class.
		* A scene could be considered it's own world or a state in a statemachine.
		*/
		class Scene
		{
		protected:
			Scene(EntityManager& entityManger) : _entityManager(entityManger) { _entity = _entityManager.Create(); };
		public:
			/** Returns the scenes entity.
			*
			*/
			const Entity& GetEntity()const { return _entity; };


			~Scene() { _entityManager.Delete(_entity); };
		protected:
			Entity _entity; /*!< The scenes own entity */
			EntityManager& _entityManager; /*!< A reference to the entitymanager created in scenemanager */
			// Create new instance of all other managers here.
		};

	}
}

#endif