#include "Ensum_input\Input.h"
#include "Safe_Delete.h"
#include "Ensum_utils\Exception.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif

namespace Ensum
{
	namespace Input
	{

		Input::Input():
			_mousePosX(0),
			_mousePosY( 0),
			_lastMousePosX( 0),
			_lastMousePosY( 0),

			_mouseLockedToScreen(false),
			_mouseLockedToCenter(false)
		{
			_keyToKey = new std::unordered_map<Keys, Keys>;
			_mousekeyToMousekey = new std::unordered_map<MouseKeys, MouseKeys>;

			memset(_keys, 0, sizeof(_keys));
			memset(_keyPressed, 0, sizeof(_keyPressed));

			memset(_mouseKeys, 0, sizeof(_mouseKeys));
			memset(_mouseKeyPressed, 0, sizeof(_mouseKeyPressed));
		}


		Input::~Input()
		{
			SAFE_DELETE(_keyToKey);
			SAFE_DELETE(_mousekeyToMousekey);
		}
		const void Input::Frame()
		{
			POINT p;

			RECT r;
			GetClientRect(_hwnd, &r);

			uint32_t wW = 800;
			uint32_t wH = 640;

			GetCursorPos(&p);

			ScreenToClient(_hwnd, &p);


			_mousePosX = (int)p.x;
			_mousePosY = (int)p.y;

			_xDiff = _lastMousePosX - _mousePosX;
			_yDiff = _lastMousePosY - _mousePosY;



			if (false)
			{
				wW = GetSystemMetrics(SM_CXSCREEN);
				wH = GetSystemMetrics(SM_CYSCREEN);
			}
			if (_mouseLockedToCenter)
			{
				_lastMousePosX = _mousePosX = wW / 2;
				_lastMousePosY = _mousePosY = wH / 2;
			/*	GetWindowRect(_hwnd, &r);
				uint32_t wX = r.left;
				uint32_t wY = r.top;*/
				p.x = _mousePosX;
				p.y = _mousePosY;
				ClientToScreen(_hwnd, &p);

				SetCursorPos(p.x, p.y);
			}
			else
			{
				_lastMousePosX = _mousePosX;
				_lastMousePosY = _mousePosY;
			}

			_scrollDelta = 0;

			memset(_keyPressed, 0, sizeof(_keyPressed));
			memset(_mouseKeyPressed, 0, sizeof(_mouseKeyPressed));
			return void();
		}
		const bool Input::IsKeyDown(Keys keyCode) const
		{
			auto find = _keyToKey->find(keyCode);
			if (find != _keyToKey->end())
				keyCode = find->second;
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_KEYS)
				Exception("Key out of range. KeyCode: " + std::to_string(key));

			return _keys[key];
		}
		const bool Input::IsKeyPushed(Keys keyCode)
		{
			auto find = _keyToKey->find(keyCode);
			if (find != _keyToKey->end())
				keyCode = find->second;
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_KEYS)
				Exception("Key out of range. KeyCode: " + std::to_string(key));
			if (!_keyPressed[key])
				return false;
			_keyPressed[key] = false;
			return true;
		}
		const bool Input::IsScrollDown(int32_t & delta)
		{
			delta = _scrollDelta;
			return _scrollDelta < 0? true : false;
		}
		const bool Input::IsScrollUp(int32_t & delta)
		{
			delta = _scrollDelta;
			return _scrollDelta > 0 ? true : false;
		}
		const bool Input::IsMouseKeyDown(MouseKeys keyCode) const
		{
			auto find = _mousekeyToMousekey->find(keyCode);
			if (find != _mousekeyToMousekey->end())
				keyCode = find->second;
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_MOUSEKEYS)
				Exception("MouseKey out of range. KeyCode: " + std::to_string(key));
			return _mouseKeys[key];
		}
		const bool Input::IsMouseKeyPushed(MouseKeys keyCode)
		{
			auto find = _mousekeyToMousekey->find(keyCode);
			if (find != _mousekeyToMousekey->end())
				keyCode = find->second;
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_MOUSEKEYS)
				Exception("MouseKey out of range. KeyCode: " + std::to_string(key));
			if (!_mouseKeyPressed[key])
				return false;
			_mouseKeyPressed[key] = false;
			return true;
		}
		const void Input::GetMousePos(int32_t & rX, int32_t & rY) const
		{
		//	auto o = System::GetOptions();
			if (false)
			{
				/*float sw = (float)GetSystemMetrics(SM_CXSCREEN);
				float sh = (float)GetSystemMetrics(SM_CYSCREEN);
				float ww = (float)o->GetScreenResolutionWidth();
				float wh = (float)o->GetScreenResolutionHeight();
				float pw = ww / sw;
				float ph = wh / sh;
				rX = static_cast<int>(_mousePosX*pw);
				rY = static_cast<int>(_mousePosY*ph);*/


			}
			else
			{
				rX = _mousePosX;
				rY = _mousePosY;
			}
			return void();
		}
		const void Input::GetMouseDiff(int32_t & rX, int32_t & rY) const
		{
			rX = _xDiff;
			rY = _yDiff;
			return void();
		}
		const void Input::LockMouseToCenter(bool lock)
		{
			if (lock)
			{
				//auto o = System::GetOptions();
				uint32_t wW = 800;
				uint32_t wH = 640;

				if (false)
				{
					wW = GetSystemMetrics(SM_CXSCREEN);
					wH = GetSystemMetrics(SM_CYSCREEN);
				}
				RECT r;
				GetWindowRect(_hwnd, &r);
				uint32_t wX = r.left;
				uint32_t wY = r.top;

				_lastMousePosX = wW / 2;
				_lastMousePosY = wH / 2;
				_mousePosX = wW / 2;
				_mousePosY = wH / 2;

				RECT rc = { 0,0,0,0 };

				AdjustWindowRect(&rc, GetWindowStyle(_hwnd), FALSE);

				SetCursorPos(wX + _mousePosX - rc.left, wY + _mousePosY - rc.top);
			}
			_mouseLockedToCenter = lock;
		}
		const void Input::LockMouseToWindow(bool lock)
		{
			if (lock)
			{
				//auto o = System::GetOptions();
				RECT clipping;
				clipping.left = 0;
				clipping.right = 800;
				clipping.top = 0;
				clipping.bottom = 640;
				if (false)
				{
					clipping.right = GetSystemMetrics(SM_CXSCREEN);
					clipping.bottom = GetSystemMetrics(SM_CYSCREEN);
					ClipCursor(&clipping);
				}
				else
				{
					RECT rc = clipping;
					AdjustWindowRect(&rc, GetWindowStyle(_hwnd), FALSE);

					RECT rcClip;           // new area for ClipCursor

					GetWindowRect(_hwnd, &rcClip);
					rcClip.right -= rc.right - clipping.right;
					rcClip.bottom -= rc.bottom - clipping.bottom;
					rcClip.left -= rc.left - clipping.left;
					rcClip.top -= rc.top - clipping.top;
					// Confine the cursor to the application's window. 

					ClipCursor(&rcClip);
				}
			}
			else
			{
				ClipCursor(nullptr);
			}
			_mouseLockedToScreen = lock;
		}
		const void Input::HideCursor(bool show) const
		{
			if (show)
			{
				while (ShowCursor(false) >= 0);
			}
			else
				while (ShowCursor(true) < 0);
		}
		const void Input::Rebind(Keys org, Keys to)
		{
			if (static_cast<uint8_t>(org) >= NUM_KEYS)
				Exception("Key out of range. KeyCode: " + std::to_string(static_cast<uint8_t>(org)));
			const uint8_t bto = static_cast<uint8_t>(to);
			if (static_cast<uint8_t>(to) >= NUM_KEYS)
				Exception("Key out of range. KeyCode: " + std::to_string(static_cast<uint8_t>(to)));

			(*_keyToKey)[org] = to;

			auto find = _keyToKey->find(to);
			if (find == _keyToKey->end())
				(*_keyToKey)[to] = Keys::None;
			return void();
		}
		const void Input::Rebind(MouseKeys org, MouseKeys to)
		{
			if (static_cast<uint8_t>(org) >= NUM_MOUSEKEYS)
				Exception("MouseKey out of range. KeyCode: " + std::to_string(static_cast<uint8_t>(org)));
			const uint8_t bto = static_cast<uint8_t>(to);
			if (static_cast<uint8_t>(to) >= NUM_MOUSEKEYS)
				Exception("MouseKey out of range. KeyCode: " + std::to_string(static_cast<uint8_t>(to)));

			(*_mousekeyToMousekey)[org] = to;

			auto find = _mousekeyToMousekey->find(to);
			if (find == _mousekeyToMousekey->end())
				(*_mousekeyToMousekey)[to] = MouseKeys::None;

			return void();
		}
		const void Input::Init(HWND hwnd)
		{
			_hwnd = hwnd;
			return void();
		}
		LRESULT Input::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
		{
			switch (umsg)
			{
			case WM_MOUSEMOVE:
				_OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				break;
			case WM_LBUTTONDOWN:
				_MouseDown(MouseKeys::LButton);
				break;
			case WM_LBUTTONUP:
				_MouseUp(MouseKeys::LButton);
				break;
			case WM_RBUTTONDOWN:
				_MouseDown(MouseKeys::RButton);
				break;
			case WM_RBUTTONUP:
				_MouseUp(MouseKeys::RButton);
				break;
			case WM_MBUTTONDOWN:
				_MouseDown(MouseKeys::MButton);
				break;
			case WM_MBUTTONUP:
				_MouseUp(MouseKeys::MButton);
				break;
			case WM_MOUSEWHEEL:
				_OnMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam));
				break;
			case WM_XBUTTONDOWN:
				switch (GET_XBUTTON_WPARAM(wParam))
				{
				case XBUTTON1:
					_MouseDown(MouseKeys::XButton1);
					break;

				case XBUTTON2:
					_MouseDown(MouseKeys::XButton2);
					break;
				}
				break;		
			case WM_XBUTTONUP:
				switch (GET_XBUTTON_WPARAM(wParam))
				{
				case XBUTTON1:
					_MouseUp(MouseKeys::XButton1);
					break;

				case XBUTTON2:
					_MouseUp(MouseKeys::XButton2);
					break;
				}
				break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				_KeyDown(static_cast<Keys>(wParam));
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				_KeyUp(static_cast<Keys>(wParam));
				break;
			case WM_KILLFOCUS:
				memset(_keys, 0, sizeof(_keys));
				memset(_keyPressed, 0, sizeof(_keyPressed));

				memset(_mouseKeys, 0, sizeof(_mouseKeys));
				memset(_mouseKeyPressed, 0, sizeof(_mouseKeyPressed));
				break;
			case WM_INPUT:
			default:
			{
				return DefWindowProc(hwnd, umsg, wParam, lParam);
			}

			}
			return 0;
			
		}
		const void Input::_KeyDown(Keys keyCode)
		{
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_KEYS)
				Exception("Key out of range. KeyCode: " + std::to_string(key));
			_keyPressed[key] = _keys[key] = true;
			return void();
		}
		const void Input::_KeyUp(Keys keyCode)
		{
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_KEYS)
				Exception("Key out of range. KeyCode: " + std::to_string(key));
			_keyPressed[key] = _keys[key] = false;
			return void();
		}
		const void Input::_MouseDown(MouseKeys keyCode)
		{
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_MOUSEKEYS)
				Exception("MouseKey out of range. KeyCode: " + std::to_string(key));
			_mouseKeyPressed[key] = _mouseKeys[key] = true;
			return void();
		}
		const void Input::_MouseUp(MouseKeys keyCode)
		{
			const uint8_t key = static_cast<uint8_t>(keyCode);
			if (key >= NUM_MOUSEKEYS)
				Exception("MouseKey out of range. KeyCode: " + std::to_string(key));
			_mouseKeyPressed[key] = _mouseKeys[key] = false;
			return void();
		}
		const void Input::_OnMouseMove(uint32_t x, uint32_t y)
		{
			_mousePosX = x;
			_mousePosY = y;
			return void();
		}
		const void Input::_OnMouseScroll(int32_t delta)
		{
			_scrollDelta = delta;
			return void();
		}
	}
}