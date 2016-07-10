#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#pragma once
#include "Ensum_components\Manager.h"
#include <DirectXMath.h>
#include "Ensum_components\TransformManager.h"
#include "Ensum_input\Input.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_inputD.lib")
#else
#pragma comment(lib, "Ensum_input.lib")
#endif

namespace Ensum
{
	namespace Components
	{
		/** Camera component, holds the view, projection, etc. For rendering.
		*
		*/
		class ENSUM_COMPONENTS_EXPORT CameraManager :
			public Manager
		{
		public:
			CameraManager(EntityManager& entManager, TransformManager& transformManager, Input::Input& input);
			~CameraManager();

			/** Create a camera which accepts player input.
			* (Player camera,etc.)
			*/
			const void CreateControllableCamera(const Entity& entity);
			/** Create a camera that only get its orientation from the translation.
			* (Security camera,etc.)
			*/
			const void CreateUnControllableCamera(const Entity& entity);
		private:
			/** The managers data struct.
			*
			*/
			struct CameraData : public ManagerMetaData
			{
				float* fov;
				float* aspect;
				float* nearp;
				float* viewDistance;

				bool* controllable;

				DirectX::XMFLOAT4X4* viewMatrix;
				DirectX::XMFLOAT4X4* projectionMatrix;
				DirectX::XMFLOAT4X4* viewProjectionMatrix;
			};
			CameraData* _datap; /*!< A reference pointer to avoid having to cast the basic datapointer all the time. */
			TransformManager& _transformManager;
			Input::Input& _input;
			private:
				/** Allocate more memory.
				*
				*/
				const void _Allocate(uint32_t size);
				/** Delete an entry in the memory block.
				* The deleted entry is replaced by the last in the block.
				*/
				const void _Destroy(uint32_t index);
				/** Takes input for player controlled camera.
				* Also calls the garbadge collection func.
				*/
				const void Frame();
				/** Called when options have changed.
				*
				*/
				const void _OnOptionsChange();
		};

	}
}

#endif