#ifndef _STATIC_MESH_MANAGER_H_
#define _STATIC_MESH_MANAGER_H_
#pragma once
#include "Manager.h"
#include "Ensum_graphics\Graphics.h"
#include "dll_export.h"
#include "Ensum_filehandler\Mesh.h"
#include <unordered_map>
#include "ensumstring.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_filehandlerD.lib")
#pragma comment(lib, "Ensum_graphicsD.lib")
#else
#pragma comment(lib, "Ensum_filehandler.lib")
#pragma comment(lib, "Ensum_graphics.lib")
#endif


namespace Ensum
{
	namespace Components
	{
		/** Handles meshes for entities.
		* Static/Non-Animated
		*/
		class ENSUM_COMPONENTS_EXPORT StaticMeshManager : public Manager
		{
		private:
			std::unordered_map<uint32_t, FileHandler::MeshData>* _loadedMeshes;
			/** The managers data struct.
			*
			*/
			struct MeshComponent : public ManagerMetaData
			{
				uint32_t* bufferID;
				uint32_t* numSubMeshes;
				uint64_t** indexStart;
				uint64_t** indexCount;
				bool ** visible;
			};
			MeshComponent* _datap; /*!< A reference pointer to avoid having to cast the basic datapointer all the time. */
		public:
			StaticMeshManager(EntityManager& entManager);
			virtual ~StaticMeshManager();
			const void CreateStaticMesh(const Entity& entity, const char* path);
		private:
			/** Allocate more memory
			* 
			*/
			const void _Allocate(uint32_t size);
			/** Delete an entry in the memory block.
			* The deleted entry is replaced by the last in the block.
			*/
			const void _Destroy(uint32_t index);
		};

	}
}

#endif