#include "Ensum_core\WindowsWindow.h"
#include "Ensum_utils\Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Safe_Delete.h"

namespace Ensum
{
	namespace Core
	{


		WinWindow::WinWindow(const Components::SceneManager& sceneManager) : Window(sceneManager), _hWnd(nullptr), _hInst(nullptr), _running(false), _wndCaption(L"Ensum"), _style(WS_OVERLAPPED | WS_CAPTION)
		{
			_input = new Input::Input;
		}

		const void WinWindow::Frame()
		{
			if (_input->IsKeyDown(Input::Keys::Escape))
				_running = false;

			_sceneManager.Frame();
		}


		WinWindow::~WinWindow()
		{
			SAFE_DELETE(_input);
		}
		const void WinWindow::Init()
		{
			_timer = new Timer();
			if (!_timer) Exception("Could not create the window timer.");

			_hInst = GetModuleHandle(NULL);

			// Setup the windows class
			WNDCLASSEX wc;

			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = _hInst;
			wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIconSm = wc.hIcon;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = _wndCaption;
			wc.cbSize = sizeof(WNDCLASSEX);

			bool fullscreen = false;
			LONG windowWidth = (LONG)800;
			LONG windowHeight = (LONG)640;


			// Register the window class.
			//Place the window in the middle of the Window.
			uint32_t windowPosX = (GetSystemMetrics(SM_CXSCREEN) - (int)windowWidth) / 2;
			uint32_t windowPosY = (GetSystemMetrics(SM_CYSCREEN) - (int)windowHeight) / 2;

			RegisterClassEx(&wc);
			RECT rc = { 0, 0, (LONG)windowWidth, (LONG)windowHeight };

			AdjustWindowRect(&rc, _style, FALSE);
			// Create the window with the Window settings and get the handle to it.
			_hWnd = CreateWindowEx(
				WS_EX_APPWINDOW,
				_wndCaption,
				_wndCaption,
				_style,
				windowPosX,
				windowPosY,
				rc.right - rc.left,
				rc.bottom - rc.top,
				NULL,
				NULL,
				_hInst,
				NULL);
			if (!_hWnd)
			{
				DWORD error = GetLastError();
				Exception("Could not create the window. Error: " + std::to_string(error));
			}

			if (fullscreen)
			{
				SetWindowLongPtr(_hWnd, GWL_STYLE,
					WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

				windowPosX = 0;
				windowPosY = 0;
				windowWidth = (LONG)GetSystemMetrics(SM_CXSCREEN);
				windowHeight = (LONG)GetSystemMetrics(SM_CYSCREEN);
				SetWindowPos(_hWnd, 0, windowPosX, windowPosY, windowWidth, windowHeight, SWP_SHOWWINDOW);
				SetForegroundWindow(_hWnd);
				SetFocus(_hWnd);


				DEVMODE dmWindowSettings;
				// If full Window set the Window to maximum size of the users desktop and 32bit.
				memset(&dmWindowSettings, 0, sizeof(dmWindowSettings));
				dmWindowSettings.dmSize = sizeof(dmWindowSettings);
				dmWindowSettings.dmPelsWidth = (unsigned long)windowWidth;
				dmWindowSettings.dmPelsHeight = (unsigned long)windowHeight;
				dmWindowSettings.dmBitsPerPel = 32;
				dmWindowSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				int r = ChangeDisplaySettings(&dmWindowSettings, 0) == DISP_CHANGE_SUCCESSFUL;
			}

			// Bring the window up on the Window and set it as main focus.
			ShowWindow(_hWnd, SW_SHOW);
			SetForegroundWindow(_hWnd);
			SetFocus(_hWnd);

			// Set the cursor to the middle of the client window
			SetCursorPos(windowPosX + windowWidth / 2, windowPosY + windowHeight / 2);


			_input->Init(_hWnd);

			return void();
		}

		const void WinWindow::Start()
		{
			_running = true;
			MSG msg;
			_timer->Start();
			while (_running)
			{
				_input->Frame();
				// Handle the windows messages.
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				FrameStart();
				//Utils::ConsoleLog::DumpToConsole("Delta: %.5f", _timer->Delta());
				// Do the frame processing.
				Frame();
			}
		}




		LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
		{
			switch (umessage)
			{
				// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}

			// Check if the window is being closed.
			case WM_CLOSE:
			{
				PostQuitMessage(0);
				break;
			}

			// All other messages pass to the message handler in the system class.
			default:
			{
				//return DefWindowProc(hwnd, umessage, wparam, lparam);
				return Window::GetInstance()->GetInput()->MessageHandler(hwnd, umessage, wparam, lparam);
			}
			}
			return 0;
		}

	}
}