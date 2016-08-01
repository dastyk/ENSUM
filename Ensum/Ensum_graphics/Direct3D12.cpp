#include "Ensum_graphics\Direct3D12.h"
#include "Exception.h"

#include "Ensum_core\WindowsWindow.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Options.h"


#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#pragma comment(lib, "Ensum_coreD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#pragma comment(lib, "Ensum_core.lib")
#endif

#ifndef ThrowIfFailed
#define ThrowIfFailed(hr) {if (FAILED(hr)){_com_error e = hr; std::string s = "%S failed in %s on line %d; error: %S"; Exception(s.c_str(),L#hr,__FILE__,__LINE__, e.ErrorMessage()); }}
#endif
namespace Ensum
{
	namespace Graphics
	{


		Direct3D12::Direct3D12() :
			_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
			_depthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT), 
			_currBackBuffer(0), 
			_swapChainBufferCount((uint8_t)Utils::Options::GetIntegerOption("Screen", "BufferCount", 2)),
			_currentFence(0)
		{
			_swapChainBuffer = new ComPtr<ID3D12Resource>[_swapChainBufferCount];
		}

		Direct3D12::~Direct3D12()
		{
			delete[] _swapChainBuffer;
		}

		const void Direct3D12::Start()
		{
#if defined(DEBUG) || defined(_DEBUG) // Enable debug layers.

			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));

			debugController->EnableDebugLayer();
#endif

			// Create DXGI Factory
			ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory)));

			ComPtr<IDXGIAdapter> adapter;
			if (_dxgiFactory->EnumAdapters(0, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC desc;
				adapter->GetDesc(&desc);

				std::wstring text = L"\n---Adapter: ";
				text += desc.Description;
				text += L"\n";

				Utils::ConsoleLog::DumpToConsole(text.c_str());
			}

			// Create hardware device

			_com_error hr = D3D12CreateDevice(
				nullptr,
				D3D_FEATURE_LEVEL_12_1,
				IID_PPV_ARGS(&_device));
			if (FAILED(hr.Error())) // Failed, fallback to WARP.
			{

				Utils::ConsoleLog::DumpToConsole("Could not create hardware device. Falling back to WARP device. Error: %S", hr.ErrorMessage());
				ComPtr<IDXGIAdapter> pWrapAdapter;
				ThrowIfFailed(_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWrapAdapter)));

				ThrowIfFailed(D3D12CreateDevice(pWrapAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)));
			}

			// Create Fence
			ThrowIfFailed(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));

			// Get descriptor sizes
			_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			_dsvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			_cbvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			// Check the quality levels
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
			msQualityLevels.Format = _backBufferFormat;
			msQualityLevels.SampleCount = 4;
			msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			msQualityLevels.NumQualityLevels = 0;
			ThrowIfFailed(_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

			Utils::ConsoleLog::DumpToConsole("\n---4X MSAA Quality Level: %d\n", msQualityLevels.NumQualityLevels);


			CreateCommandObjects();
			CreateSwapChain();
			CreateRtvAndDsvDescriptorHeaps();
			CreateRenderTargetViews();
			CreateDepthStencilBufferView();


			ResetCommandLists();

			_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				_depthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE));

			CloseCommandLists();

			return void();  
		}

		const void Direct3D12::Shutdown()
		{
			return void();
		}
		const void Direct3D12::ResetCommandLists()
		{
			ThrowIfFailed(_cmdListAlloc->Reset());
			ThrowIfFailed(_cmdList->Reset(_cmdListAlloc.Get(), nullptr));
			return void();
		}
		const void Direct3D12::CloseCommandLists()
		{
			ThrowIfFailed(_cmdList->Close());
			
			ID3D12CommandList* cmdsLists[] = { _cmdList.Get() };
			_cmdQueue->ExecuteCommandLists(1, cmdsLists);

			return void();
		}

		const void Direct3D12::SwapBackBuffer(uint32_t syncInterval, uint32_t flags)
		{
			ThrowIfFailed(_swapChain->Present(syncInterval, flags));
			_currBackBuffer = (_currBackBuffer + 1) % _swapChainBufferCount;
			return void();
		}

		const void Direct3D12::FlushCommandQueue()
		{
			_currentFence++;

			ThrowIfFailed(_cmdQueue->Signal(_fence.Get(), _currentFence));

			if (_fence->GetCompletedValue() < _currentFence)
			{
				HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

				ThrowIfFailed(_fence->SetEventOnCompletion(_currentFence, eventHandle));

				WaitForSingleObject(eventHandle, INFINITE);
				CloseHandle(eventHandle);
			}
			return void();
		}

		const void Direct3D12::TransitionBackBufferToRenderTarget()
		{
		
			_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				_CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_PRESENT, 
				D3D12_RESOURCE_STATE_RENDER_TARGET));
			return void();
		}
		const void Direct3D12::TransitionBackBufferToPresent()
		{
			_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
				_CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT));
			return void();
		}

		const void Direct3D12::SetViewPort(const D3D12_VIEWPORT * vp, uint8_t numViewPorts)
		{
			_cmdList->RSSetViewports(numViewPorts, vp);
			return void();
		}

		const void Direct3D12::ClearBackBuffer(const float colorRGBA[4], uint32_t numRects, const D3D12_RECT * pRects)
		{
			_cmdList->ClearRenderTargetView(_CurrentBackBufferView(), colorRGBA, numRects, pRects);
			return void();
		}

		const void Direct3D12::ClearDepthStencilView(D3D12_CLEAR_FLAGS flags, float depth, uint8_t stencil, uint32_t numRects, const D3D12_RECT * pRects)
		{
			_cmdList->ClearDepthStencilView(_DepthStencilView(), flags, depth, stencil, numRects, pRects);
			return void();
		}

		const void Direct3D12::SetBackBufferAsRenderTarget(bool specDepthStencilView, const D3D12_CPU_DESCRIPTOR_HANDLE sdsv)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = specDepthStencilView ? sdsv : _DepthStencilView();
			_cmdList->OMSetRenderTargets(1, &_CurrentBackBufferView(), true, &dsv);
			return void();
		}
		ComPtr<ID3D12Resource> Direct3D12::CreateDefaultBuffer(const void * initData, uint64_t byteSize, ComPtr<ID3D12Resource>& uploadBuffer)
		{
			ComPtr<ID3D12Resource> dB;

			ThrowIfFailed(_device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&dB)
			));

			ThrowIfFailed(_device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadBuffer)
			));

			D3D12_SUBRESOURCE_DATA data = {};
			data.pData = initData;
			data.SlicePitch = data.RowPitch = byteSize;


			_cmdList->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(
					dB.Get(),
					D3D12_RESOURCE_STATE_COMMON,
					D3D12_RESOURCE_STATE_COPY_DEST));

			UpdateSubresources<1>(
				_cmdList.Get(),
				dB.Get(), uploadBuffer.Get(),
				0, 0, 1,
				&data);

			_cmdList->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(
					dB.Get(),
					D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_GENERIC_READ));

			return dB;
		}
		D3D12_CPU_DESCRIPTOR_HANDLE Direct3D12::_CurrentBackBufferView() const
		{
			return CD3D12_CPU_DESCRIPTOR_HANDLE(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _currBackBuffer, _rtvDescriptorSize);
		}
		D3D12_CPU_DESCRIPTOR_HANDLE Direct3D12::_DepthStencilView() const
		{
			return _dsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
		ID3D12Resource * Direct3D12::_CurrentBackBuffer() const
		{
			return _swapChainBuffer[_currBackBuffer].Get();
		}
		
		const void Direct3D12::CreateCommandObjects()
		{

			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			ThrowIfFailed(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue)));

			ThrowIfFailed(_device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(&_cmdListAlloc)));

			ThrowIfFailed(_device->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				_cmdListAlloc.Get(),
				nullptr,
				IID_PPV_ARGS(&_cmdList)));

			// Start in closed state, we need this because we will call reset, which needs the cmdList to be closed.
			_cmdList->Close();
			return void();
		}
		const void Direct3D12::CreateSwapChain()
		{
			_swapChain.Reset();

			DXGI_SWAP_CHAIN_DESC sd;
			sd.BufferDesc.Width = (uint32_t)Utils::Options::GetIntegerOption("Screen", "Width", 800);
			sd.BufferDesc.Height = (uint32_t)Utils::Options::GetIntegerOption("Screen", "Height", 640);
			sd.BufferDesc.RefreshRate.Numerator = (uint32_t)Utils::Options::GetIntegerOption("Screen", "RefreshRateNumerator", 60);
			sd.BufferDesc.RefreshRate.Denominator = (uint32_t)Utils::Options::GetIntegerOption("Screen", "RefreshRateDenominator", 1);
			sd.BufferDesc.Format = _backBufferFormat;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			sd.SampleDesc.Count = Utils::Options::GetBooleanOption("Screen", "4xMsaa", false) ? 4 : 1;
			sd.SampleDesc.Quality = Utils::Options::GetBooleanOption("Screen", "4xMsaa", false) ? (_4xMsaaQuality - 1) : 0;

			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = _swapChainBufferCount;

			sd.OutputWindow = Core::WinWindow::GetWindow();
			sd.Windowed = true;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			ThrowIfFailed(_dxgiFactory->CreateSwapChain(_cmdQueue.Get(), &sd, _swapChain.GetAddressOf()));


			return void();
		}
		const void Direct3D12::CreateRtvAndDsvDescriptorHeaps()
		{
			D3D12_DESCRIPTOR_HEAP_DESC rtvhd;
			rtvhd.NumDescriptors = _swapChainBufferCount;
			rtvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvhd.NodeMask = 0;

			ThrowIfFailed(_device->CreateDescriptorHeap(&rtvhd, IID_PPV_ARGS(&_rtvHeap)));

			D3D12_DESCRIPTOR_HEAP_DESC dsvhd;
			dsvhd.NumDescriptors = 1;
			dsvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsvhd.NodeMask = 0;
			ThrowIfFailed(_device->CreateDescriptorHeap(&dsvhd, IID_PPV_ARGS(&_dsvHeap)));

			return void();
		}
		const void Direct3D12::CreateRenderTargetViews()
		{

			CD3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());

			for (uint8_t i = 0; i < _swapChainBufferCount; i++)
			{
				ThrowIfFailed(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_swapChainBuffer[i])));

				_device->CreateRenderTargetView(_swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

				rtvHeapHandle.Offset(1, _rtvDescriptorSize);
			}

			return void();
		}

		const void Direct3D12::CreateDepthStencilBufferView()
		{
			D3D12_RESOURCE_DESC dsd;
			dsd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			dsd.Alignment = 0;
			dsd.Width = (uint64_t)Utils::Options::GetIntegerOption("Screen", "Width", 800);
			dsd.Height = (uint64_t)Utils::Options::GetIntegerOption("Screen", "Height", 640);
			dsd.DepthOrArraySize = 1;
			dsd.MipLevels = 1;
			dsd.Format = _depthStencilFormat;

			dsd.SampleDesc.Count = Utils::Options::GetBooleanOption("Screen", "4xMsaa", false) ? 4 : 1;
			dsd.SampleDesc.Quality = Utils::Options::GetBooleanOption("Screen", "4xMsaa", false) ? (_4xMsaaQuality - 1) : 0;

			dsd.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			dsd.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			D3D12_CLEAR_VALUE optClear;
			optClear.Format = _depthStencilFormat;
			optClear.DepthStencil.Depth = 1.0f;
			optClear.DepthStencil.Stencil = 0;
			ThrowIfFailed(_device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&dsd,
				D3D12_RESOURCE_STATE_COMMON,
				&optClear,
				IID_PPV_ARGS(&_depthStencilBuffer)));

			_device->CreateDepthStencilView(_depthStencilBuffer.Get(), nullptr, _DepthStencilView());




			return void();
		}

	}
}