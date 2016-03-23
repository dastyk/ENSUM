#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#pragma once
#include "Ensum_components\Scene.h"
#include "Manager.h"

namespace Ensum
{
	namespace Components
	{
		/** Manages all the scenes.
		*
		*/

		class ENSUM_COMPONENTS_EXPORT SceneManager : private Manager
		{
		private:
			struct SceneData : ManagerMetaData
			{
				Scene** scenePtr;
				bool* sceneUpdate;
			};
		public:
			SceneManager();
			~SceneManager();

			/** Create the scene and return it's entity.
			*
			*/
			const Entity& CreateScene(Scene* scene);
		/*	template <typename SceneType>
			const Entity& CreateScene(Input::Input* input);*/
			/** Remove the scene component from the entity.
			*
			*/
			const void RemoveScene(const Entity& sceneEntity);
			/** Specifies whether the scene should be updated each frame.
			*
			*/
			const void SetUpdate(const Entity& sceneEntity, bool update);

			/** Do the framework for all active scenes.
			*
			*/
			const void Frame();

			/** Return a reference to the entity manager.
			*
			*/
			EntityManager& GetEntityManager();
		private:
			/** Allocate more memory for scenedata
			*
			*/
			const void _Allocate(uint32_t size);
			/** Delete an entry in the memory block.
			* The deleted entry is replaced by the last in the block.
			*/
			const void _Destroy(uint32_t index);
			/** Looks for destroyed entities and deletes them from the data block.
			* 
			*/
		private:
			EntityManager _entManager;
			Input::Input* _input;
			SceneData* _datap;
		};

		//ENSUM_COMPONENTS_TEMPLATE template ENSUM_COMPONENTS_EXPORT const Entity& SceneManager::CreateScene<NullScene>(Input::Input* input);

	}
}
#endif