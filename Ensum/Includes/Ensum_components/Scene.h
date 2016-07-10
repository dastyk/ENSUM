#ifndef _SCENE_H_
#define _SCENE_H_

#pragma once
#include "Ensum_components\TransformManager.h"
#include "Ensum_components\EntityManager.h"
#include "Ensum_components\CameraManager.h"
#include "Ensum_components\DataManager.h"
#include "Ensum_input\Input.h"
#include "dll_export.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_inputD.lib")
#else
#pragma comment(lib, "Ensum_input.lib")
#endif


namespace Ensum
{
	namespace Components
	{
		/** The abstract scene class.
		* A scene could be considered it's own world or a state in a statemachine.
		*/
		class ENSUM_COMPONENTS_EXPORT Scene
		{
		protected:
			Scene(EntityManager& entityManger, Input::Input* input) :
				_entityManager(entityManger), 
				_input(input), 
				_data(entityManger),
				_transform(entityManger),
				_camera(entityManger, _transform, *_input),
				_managers(new std::vector<Manager*>)
			{ 
				_entity = _entityManager.Create(); 
				_managers->push_back(&_data);
				_managers->push_back(&_transform);
				_managers->push_back(&_camera);
			};
		public:
			/** Returns the scenes entity.
			*
			*/
			virtual const Entity& GetEntity()const { return _entity; };
			/** The frame function for the scene.
			*
			*/
			virtual const void Frame() 
			{
				for (auto& m : *_managers)
				{
					m->Frame();
				}
			};

			virtual ~Scene() { _entityManager.Delete(_entity); delete _managers; };
		protected:
			Entity _entity; /*!< The scenes own entity */
			EntityManager& _entityManager; /*!< A reference to the entitymanager created in scenemanager */
			Input::Input* _input;
			// Create new instance of all other managers here.
			std::vector<Manager*>* _managers;
			DataManager _data;
			TransformManager _transform;
			CameraManager _camera;
		};

	}
}

#endif