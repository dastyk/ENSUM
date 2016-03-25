#include "Ensum_graphics\D3D11.h"
#include "Safe_Delete.h"
#include "Exception.h"
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

		D3D11::D3D11() : Graphics("DirectX11")
		{
			
		}


		D3D11::~D3D11()
		{
			
		}
		const void D3D11::OnCreateDevice()
		{
			_D3D11.Start();
		}
		const void D3D11::OnDestroyDevice()
		{
			_D3D11.Shutdown();
		}
		const void D3D11::_OnOptionsChange()
		{
			WaitForSingleObject(_optionsChangeMutex, INFINITE);
			bool f = _fullscreen;
			unsigned w = _width;
			unsigned h = _height;

			_fullscreen = Utils::Options::GetBooleanOption("Screen", "Fullscreen", false);
			_width = (unsigned)Utils::Options::GetIntegerOption("Screen", "Width", 800);
			_height = (unsigned)Utils::Options::GetIntegerOption("Screen", "Height", 640);
			_vsync = Utils::Options::GetBooleanOption("Graphics", "Vsync", false);

			if (_fullscreen != f || _width != w || _height != h)
			{
			
				_D3D11.Resize();
			}
			ReleaseMutex(_optionsChangeMutex);
			
		}
		const void D3D11::_BeginFrame(void)
		{
			ID3D11DeviceContext* deviceContext = _D3D11.GetDeviceContext();
			if (!deviceContext)
				return;

			static float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };

			deviceContext->ClearRenderTargetView(_D3D11.GetBackBufferRTV(), clearColor);
		}
		const void D3D11::_EndFrame(void)
		{
			_D3D11.GetSwapChain()->Present(_vsync, 0);
		}
		const void D3D11::_Frame()
		{
			
			return void();
		}
	}
}