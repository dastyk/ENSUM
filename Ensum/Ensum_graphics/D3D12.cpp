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
			_viewPort.Width = (uint32_t)Utils::Options::GetIntegerOption("Screen", "Width", 800);
			_viewPort.Height = (uint32_t)Utils::Options::GetIntegerOption("Screen", "Height", 640);
			_viewPort.MinDepth = 0.0f;
			_viewPort.MaxDepth = 1.0f;

			return void();
		}

		const void D3D12::OnDestroyDevice()
		{
			_D3D12.Shutdown();
			return void();
		}

		const void D3D12::_OnOptionsChange()
		{
			return void();
		}

		const void D3D12::_BeginFrame(void)
		{
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

			_D3D12.FlushCommandQueue();
			return void();
		}

		const void D3D12::_Frame()
		{
			return void();
		}
	}
}