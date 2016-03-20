#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#pragma once
#include "Ensum_components\Scene.h"
#include <unordered_map>

namespace Ensum
{
	namespace Components
	{
		/** Manages all the scenes.
		*
		*/
		class SceneManager
		{
		public:
			SceneManager();
			~SceneManager();

		private:
			EntityManager _entityManager;
			std::unordered_map<Entity, Scene*, EntityHasher> _entityToScene;
		};
	}
}
#endif