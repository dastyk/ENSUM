#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include <stdint.h>
#include <vector>
#include "dll_export.h"
#include <fstream>
using namespace std;

namespace Ensum
{
	namespace FileHandler
	{
		struct ENSUM_FILEHANDLER_EXPORT MeshData
		{
			uint32_t bufferID;
			uint32_t numSubMeshes;
			uint64_t* indexStart;
			uint64_t* indexCount;
			MeshData() : bufferID(-1), numSubMeshes(-1), indexStart(nullptr), indexCount(nullptr)
			{

			}
			MeshData(const MeshData& other)
			{
				bufferID = other.bufferID;
				numSubMeshes = other.numSubMeshes;
				indexStart = new uint64_t[numSubMeshes];
				indexCount = new uint64_t[numSubMeshes];
				memcpy(indexStart, other.indexStart, numSubMeshes);
				memcpy(indexCount, other.indexCount, numSubMeshes);
			}
			MeshData& operator=(const MeshData& other)
			{
				bufferID = other.bufferID;
				numSubMeshes = other.numSubMeshes;
				delete[] indexStart;
				delete[] indexCount;
				indexStart = new uint64_t[numSubMeshes];
				indexCount = new uint64_t[numSubMeshes];
				memcpy(indexStart, other.indexStart, numSubMeshes);
				memcpy(indexCount, other.indexCount, numSubMeshes);
				return *this;
			}
			~MeshData()
			{
				delete[] indexStart;
				delete[] indexCount;
			}
		};
		struct ENSUM_FILEHANDLER_EXPORT Position
		{
			float x, y, z, w;
			Position(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
			{}
		};

		struct ENSUM_FILEHANDLER_EXPORT TexCoord
		{
			float u, v, w;
			TexCoord(float u, float v, float w) : u(u), v(v), w(w)
			{}
		};

		struct ENSUM_FILEHANDLER_EXPORT Normal
		{
			float x, y, z;
			Normal(float x, float y, float z) : x(x), y(y), z(z)
			{}
		};

		struct ENSUM_FILEHANDLER_EXPORT Indices
		{
			uint8_t indexCount;
			uint64_t index[4];// array of size 4.
		};

		struct ENSUM_FILEHANDLER_EXPORT Face
		{
			uint8_t indexCount;
			Indices indices[4]; // Array of size 4.
			Face(vector<vector<uint64_t>>& face) : indexCount(0)
			{
				indexCount = face.size();

				for (uint8_t i = 0; i < face.size(); i++)
				{
					indices[i].indexCount = face[i].size();
					for (uint8_t j = 0; j < face[i].size(); j++)
					{
						indices[i].index[j] = face[i][j];
					}
				}
			}

		};

		struct ENSUM_FILEHANDLER_EXPORT SubMesh
		{
			char name[100];
			uint64_t faceStart;
			uint64_t faceCount;
		};

		struct ENSUM_FILEHANDLER_EXPORT Data
		{
			void* buffer = nullptr;
			size_t allocated = 0;

			size_t PosStart = 0;
			uint64_t NumPos = 0;
			uint64_t PosCap = 0;

			size_t TexStart = 0;
			uint64_t NumTex = 0;
			uint64_t TexCap = 0;

			size_t NormStart = 0;
			uint64_t NumNorm = 0;
			uint64_t NormCap = 0;

			size_t FaceStart = 0;
			uint64_t NumFace = 0;
			uint64_t FaceCap = 0;

			size_t SubMeshStart = 0;
			uint32_t NumSubMesh = 0;
			uint32_t SubMeshCap = 0;
		};

		class ENSUM_FILEHANDLER_EXPORT Mesh
		{
		protected:
			Data _data;
		public:
			Mesh(const char* path);
			Mesh(const Mesh& other);
			virtual ~Mesh();
		};

	}
}

#endif