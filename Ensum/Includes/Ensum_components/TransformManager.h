#ifndef _TRANSFORM_MANAGER_H_
#define _TRANSFORM_MANAGER_H_

#pragma once
#include "Ensum_components\Manager.h"
#include <DirectXMath.h>
#include "dll_export.h"

namespace Ensum
{
	namespace Components
	{
		/** Handles the transform of the entities.
		*
		*/
		class ENSUM_COMPONENTS_EXPORT TransformManager :
			public Manager
		{
		//	friend class PhysicsComp; // Physics comp now have access to private members in transformmanager.
		private:
			/** The managers data struct.
			*
			*/
			struct TransformComponents : public ManagerMetaData
			{
				uint32_t *Parent; // Parent transform (whose reference system we are relative to).
				uint32_t *FirstChild; // Use this to get other children
				uint32_t *PrevSibling; // The previous child of our parent
				uint32_t *NextSibling; // The next child of our parent

				DirectX::XMFLOAT4X4* Local; // Local transform with respect to parent
				DirectX::XMFLOAT4X4* World; // Concatenation of local and parent World (final world)

				DirectX::XMFLOAT3* PositionL;
				DirectX::XMFLOAT3* Rotation;
				DirectX::XMFLOAT3* Scale;

				DirectX::XMFLOAT3* Forward;
				DirectX::XMFLOAT3* Up;
				DirectX::XMFLOAT3* Right;
			};
			TransformComponents* _datap; /*!< A reference pointer to avoid having to cast the basic datapointer all the time. */
		public:
			TransformManager(EntityManager& entManager);
			~TransformManager();

			/** Create a transform component for the entity.
			*
			*/
			const void CreateTransform(const Entity& entity);
		
			/** Bind a entity as child to another.
			* If relativeToParent is true, the current local pos of the child will be the same.
			* If relativeToParent is false, the local pos of the child will be translated to the parents space.
			* Think of it as welding the entity to the parent.
			*/
			const void BindChild(const Entity& parent, const Entity& child, bool relativeToParent = false);
			/** Unbind an entity as child.
			* The childs local transform will be translated to world pos.
			* So it does not teleport away.
			*/
			const void UnbindChild(const Entity& child);



			const void MoveForward(const Entity& entity, const float amount);
			const void MoveBackward(const Entity& entity, const float amount);
			const void MoveRight(const Entity& entity, const float amount);
			const void MoveLeft(const Entity& entity, const float amount);
			const void MoveUp(const Entity& entity, const float amount);
			const void MoveDown(const Entity& entity, const float amount);
			const void MoveAlongVector(const Entity& entity, const DirectX::XMVECTOR amount);
			const void MoveAlongVector(const Entity& entity, DirectX::XMVECTOR dir, float amount);
			const void RotateYaw(const Entity& entity, const float radians);
			const void RotatePitch(const Entity& entity, const float radians);
			const void RotateRoll(const Entity& entity, const float radians);

			const void SetPosition(const Entity& entity, const DirectX::XMFLOAT3& position);
			const void SetPosition(const Entity& entity, const DirectX::XMVECTOR& position);
			const void SetRotation(const Entity& entity, const DirectX::XMFLOAT3& rotation);
			const void SetRotation(const Entity& entity, const DirectX::XMVECTOR& rotation);
			const void SetScale(const Entity& entity, const DirectX::XMFLOAT3& scale);
			const void SetScale(const Entity& entity, const DirectX::XMVECTOR& scale);
			const void SetForward(const Entity& entity, const DirectX::XMFLOAT3& forward);
			const void SetForward(const Entity& entity, const DirectX::XMVECTOR& forward);

			const DirectX::XMVECTOR GetPosition(const Entity& entity);
			const DirectX::XMVECTOR GetRotation(const Entity& entity);
			const DirectX::XMVECTOR GetScale(const Entity& entity);
			const DirectX::XMVECTOR GetForward(const Entity& entity);
			const DirectX::XMVECTOR GetRight(const Entity& entity);
			const DirectX::XMVECTOR GetUp(const Entity& entity);

		
		private:
			/** Allocate more memory
			*
			*/
			const void _Allocate(uint32_t size);
			/** Delete an entry in the memory block.
			* The deleted entry is replaced by the last in the block.
			*/
			const void _Destroy(uint32_t index);
			/** Bind a entity as child to another. (The private version.)
			* If relativeToParent is true, the current local pos of the child will be the same.
			* If relativeToParent is false, the local pos of the child will be translated to the parents space.
			* It is assumed that the indices exist.
			* Think of it as welding the entity to the parent.
			*/
			const void _BindChild(uint32_t parent, uint32_t child, bool relativeToParent);
			/** Unbind an entity as child.(The private version.)
			* The childs local transform will be translated to world pos.
			* So it does not teleport away.
			*  It is assumed that the indices exist.
			*/
			const void _UnbindChild(uint32_t child);

			/** Transform to parents local.  
			* Eg. If child is at pos (1,0) and parent at (2,0), we want the child local to be (-1,0) but world to be (1,0)
			* It is assumed that the indices exist.
			*/
			const void _TransformToParentLocal(uint32_t parent, uint32_t child);
			/** Transform from parents local.
			* (The reverse of the above.)
			* It is assumed that the indices exist.
			*/
			const void _TransformFromParentLocal(uint32_t parent, uint32_t child);
			/** Transform has changed, so update everything. 
			* It is assumed that the indices exist.
			*/
			const void _Transform(uint32_t index);
		};
	}
}
#endif