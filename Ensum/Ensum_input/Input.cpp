#include "Ensum_input\Input.h"
#include "Safe_Delete.h"

namespace Ensum
{
	namespace Input
	{

		Input::Input()
		{
			_keyboard = new std::unique_ptr<DirectX::Keyboard>;
			_mouse = new 	std::unique_ptr<DirectX::Mouse>;

			_keyToKey = new std::unordered_map<DirectX::Keyboard::Keys, DirectX::Keyboard::Keys>;
			_mousekeyToMousekey = new std::unordered_map<DirectX::Mouse::Keys, DirectX::Mouse::Keys>;


		}


		Input::~Input()
		{
			SAFE_DELETE(_keyboard);
			SAFE_DELETE(_mouse);
			
			SAFE_DELETE(_keyToKey);
			SAFE_DELETE(_mousekeyToMousekey);
		}
		const void Input::Init(HWND hwnd)
		{
			(*_keyboard) = std::make_unique<DirectX::Keyboard>();
			(*_mouse) = std::make_unique<DirectX::Mouse>();
			(*_mouse)->SetWindow(hwnd);
			return void();
		}
		LRESULT Input::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
		{
			switch (umsg)
			{

			case WM_ACTIVATEAPP:

				DirectX::Keyboard::ProcessMessage(umsg, wParam, lParam);
				DirectX::Mouse::ProcessMessage(umsg, wParam, lParam);
				break;


			case WM_INPUT:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			case WM_MOUSEHOVER:
				DirectX::Mouse::ProcessMessage(umsg, wParam, lParam);
				break;

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
				DirectX::Keyboard::ProcessMessage(umsg, wParam, lParam);
				break;
			default:
			{
				return DefWindowProc(hwnd, umsg, wParam, lParam);
			}

			}
			return 0;
			
		}
		bool Input::Escape()
		{
			auto kb = (*_keyboard)->GetState();
			if (kb.Escape)
				return true;
		}
	}
}