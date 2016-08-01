#include "Ensum_graphics\D3D12.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Options.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif

namespace Ensum
{
	namespace Graphics
	{

		D3D12::D3D12() : Graphics("DirectX12")
		{
		}


		D3D12::~D3D12()
		{
		}

		const void D3D12::OnCreateDevice()
		{
			
			_D3D12.Start();

			_viewPort.TopLeftX = 0.0f;
			_viewPort.TopLeftY = 0.0f;
			_viewPort.Width = (float)Utils::Options::GetIntegerOption("Screen", "Width", 800);
			_viewPort.Height = (float)Utils::Options::GetIntegerOption("Screen", "Height", 640);
			_viewPort.MinDepth = 0.0f;
			_viewPort.MaxDepth = 1.0f;
			return void();
		}

		const void D3D12::OnDestroyDevice()
		{
			_D3D12.Shutdown();
			return void();
		}

//		const D3D12::MeshData* D3D12::LoadMesh(char* path, uint32_t& id, uint32_t flag)
//		{
//			string t = path;
//			id = t.GetHash();
//			auto& find = _loadedMeshes.find(id);
//			if (find == _loadedMeshes.end())
//			{
//				auto& lmesh = _loadedMeshes[id];
//
//
//				FileHandler::Mesh mesh(path);
//				void* vertexData, *indexData;
//				uint64_t vertexDataSize, indexDataSize;
//				ComPtr<ID3D12Resource> vertexUp, indexUp;
//
//				_InterleaveVertexData(mesh, &vertexData, vertexDataSize, &indexData, indexDataSize);
//
//				lmesh.vertexBuffer.buffer = _D3D12.CreateDefaultBuffer(vertexData, vertexDataSize, vertexUp);
//				//lmesh.indexBuffer.buffer = _D3D12.CreateDefaultBuffer(indexData, indexDataSize, indexUp);
//
//				lmesh.vertexBuffer.view.BufferLocation = lmesh.vertexBuffer.buffer->GetGPUVirtualAddress();
//				lmesh.vertexBuffer.view.StrideInBytes = sizeof(Vertex);
//				lmesh.vertexBuffer.view.SizeInBytes = vertexDataSize;
//
//				lmesh.indexBuffer.view.BufferLocation = lmesh.indexBuffer.buffer->GetGPUVirtualAddress();
//				lmesh.indexBuffer.view.Format = DXGI_FORMAT_R32_UINT;
//				lmesh.indexBuffer.view.SizeInBytes = indexDataSize;
//
//				lmesh.flag = flag;
//				lmesh.subs++;
//
//				return &lmesh;
//
//			}
//	
//			return &find->second;
//			
//		}

	/*	const void D3D12::_InterleaveVertexData(const FileHandler::Mesh &mesh, void ** vertexData, uint64_t & vertexDataSize, void ** indexData, uint64_t & indexDataSize)
		{
			return void();
		}
*/
		const void D3D12::_OnOptionsChange()
		{
			return void();
		}

		const void D3D12::_BeginFrame(void)
		{
			_D3D12.FlushCommandQueue();
			_D3D12.ResetCommandLists();

			_D3D12.TransitionBackBufferToRenderTarget();

			_D3D12.SetViewPort(&_viewPort);

			float color[] = { 0.2f, 0.2f, 0.6f, 1.0f };
			_D3D12.ClearBackBuffer(color);
			_D3D12.ClearDepthStencilView();



			return void();
		}

		const void D3D12::_EndFrame(void)
		{
			_D3D12.TransitionBackBufferToPresent();

			_D3D12.CloseCommandLists();

			_D3D12.SwapBackBuffer();

			
			return void();
		}

		const void D3D12::_Frame()
		{
			return void();
		}
	}
}