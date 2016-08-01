#ifndef _D3D12_H_
#define _D3D12_H_

#pragma once
#include "Direct3D12.h"
#include "Ensum_graphics\Graphics.h"
#include "Data_Meta.h"
#include <unordered_map>

#include <DirectXMath.h>


namespace Ensum
{
	namespace Graphics
	{

		class ENSUM_GRAPHICS_EXPORT D3D12 : public Graphics
		{
		protected:
			struct Vertex
			{
				DirectX::XMFLOAT3 Position;
				DirectX::XMFLOAT2 TexCoord;
				DirectX::XMFLOAT3 Normal;
			};
			struct BufferData
			{
				Direct3D12::VertexBuffer vertexBuffer;
				Direct3D12::IndexBuffer indexBuffer;
			};
			
		public:
			D3D12();
			virtual ~D3D12();

			/** Create all the graphics stuff.
			*
			*/
			virtual const void OnCreateDevice();
			/** Destroy all the graphics stuff.
			*
			*/
			virtual const void OnDestroyDevice();
			/** Loads a mesh and creates a vertex and indexbuffer
			* ID of mesh is returned.
			* Use flag to indicate loading and unloading for mesh.
			*/
		protected:
			/** Interleave the vertex data
			* 
			*/
		//	virtual const void _InterleaveVertexData(const FileHandler::Mesh &mesh, void **vertexData, uint64_t& vertexDataSize, void **indexData, uint64_t& indexDataSize);
		protected:
			/** Options change func.
			*
			*/
			virtual const void _OnOptionsChange();

			/** Begin the frame.
			*
			*/
			virtual const void _BeginFrame(void);
			/** End the frame.
			*
			*/
			virtual const void _EndFrame(void);

			/** Render everything.
			*
			*/
			virtual const void _Frame();

		protected:
			Direct3D12 _D3D12;
		protected:
			D3D12_VIEWPORT _viewPort;
		};
	}
}
#endif