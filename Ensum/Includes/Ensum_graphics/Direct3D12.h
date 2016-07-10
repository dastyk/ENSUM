#ifndef _DIRECT3D12_H_
#define _DIRECT3D12_H_

#pragma once

#include "dll_export.h"
#include <wrl\client.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_5.h>
#include <comdef.h>
#include <stdint.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
using namespace Microsoft;
using namespace WRL;

namespace Ensum
{
	namespace Graphics
	{
		

		class ENSUM_GRAPHICS_EXPORT Direct3D12
		{
		private:
				

			struct CD3D12_CPU_DESCRIPTOR_HANDLE
			{
				D3D12_CPU_DESCRIPTOR_HANDLE handle;
				CD3D12_CPU_DESCRIPTOR_HANDLE(D3D12_CPU_DESCRIPTOR_HANDLE h) : handle(h)
				{
				}
				CD3D12_CPU_DESCRIPTOR_HANDLE(D3D12_CPU_DESCRIPTOR_HANDLE h, size_t curr, size_t size)
				{
					handle.ptr = h.ptr + curr * size;
				}
				const void Offset(size_t curr, size_t size)
				{
					handle.ptr += curr*size;
				}
				operator D3D12_CPU_DESCRIPTOR_HANDLE()
				{
					return handle;
				}
			};

		public:
			Direct3D12();
			~Direct3D12();

			/** Init DirectX12.
			*
			*/
			const void Start();
			/** Shutdown everything.
			*
			*/
			const void Shutdown();
			/** Reset command lists.
			* Reset the commandlistallocator and commandlist.
			*/
			const void ResetCommandLists();
			/** Closes the commandlist from more commands.
			* This also executes the commands.
			*/
			const void CloseCommandLists();
			/** Swap the back and front buffer.
			* 
			*/
			const void SwapBackBuffer(uint32_t syncInterval = 0, uint32_t flags = 0);
			/** Indicates a state transition from present to render target on the backbuffer resource.
			*
			*/
			const void FlushCommandQueue();
			const void TransitionBackBufferToRenderTarget();
			/** Indicates a state transition from render target to present on the backbuffer resource.
			*
			*/
			const void TransitionBackBufferToPresent();
			/** Sets the given view port.
			*
			*/
			const void SetViewPort(const D3D12_VIEWPORT* vp, uint8_t numViewPorts = 1);
			/** Clears the current backbuffer.
			* pRects specifices the areas to clear.
			*/
			const void ClearBackBuffer(const float colorRGBA[4], uint32_t numRects = 0, const D3D12_RECT* pRects = nullptr);
			/** Clears the current backbuffer.
			* pRects specifices the areas to clear.
			*/
			const void ClearDepthStencilView(const D3D12_CLEAR_FLAGS flags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, float depth = 1.0f, uint8_t stencil = 0, uint32_t numRects = 0, const D3D12_RECT* pRects = nullptr);
			/** Set the rendertarget to the current backbuffer.
			* If specDepthStencilView is true then depthstencilview in sdsv will be used instead of default.
			*/
			const void SetBackBufferAsRenderTarget(bool specDepthStencilView = false, const D3D12_CPU_DESCRIPTOR_HANDLE sdsv = D3D12_CPU_DESCRIPTOR_HANDLE());
		private:
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
			ID3D12Resource* CurrentBackBuffer()const;
		private:
			/** Create the commands objects.
			* This includes the CommandQueue, CommandListAllocator and CommandList.
			*/
			const void CreateCommandObjects();
			/** Create the swapchain
			*
			*/
			const void CreateSwapChain();
			/** Create DesciptorHeaps for RTV and DSV.
			*
			*/
			const void CreateRtvAndDsvDescriptorHeaps();
			/** Create Render target views for swapchain.
			*
			*/
			const void CreateRenderTargetViews();
			/** Create the Depth stencil buffer/view.
			*
			*/
			const void CreateDepthStencilBufferView();
		private:
			ComPtr<IDXGIFactory4> _dxgiFactory;
			ComPtr<ID3D12Device> _device;
			ComPtr<ID3D12Fence> _fence;
			ComPtr<ID3D12CommandQueue> _cmdQueue;
			ComPtr<ID3D12CommandAllocator> _cmdListAlloc;
			ComPtr<ID3D12GraphicsCommandList> _cmdList;
			ComPtr<IDXGISwapChain> _swapChain;
			ComPtr<ID3D12DescriptorHeap> _rtvHeap;
			ComPtr<ID3D12DescriptorHeap> _dsvHeap;
			ComPtr<ID3D12Resource>* _swapChainBuffer;
			ComPtr<ID3D12Resource> _depthStencilBuffer;
		private:
			DXGI_FORMAT _backBufferFormat;
			DXGI_FORMAT _depthStencilFormat;

			uint8_t _currBackBuffer;
			uint8_t _swapChainBufferCount;

			uint64_t _currentFence;
			uint32_t _rtvDescriptorSize;
			uint32_t _dsvDescriptorSize;
			uint32_t _cbvDescriptorSize;
			uint32_t _4xMsaaQuality;
		};

	}
}

#endif