#ifndef _WINDOWS_WINDOW_H_
#define _WINDOWS_WINDOW_H_
#pragma once
#include "Ensum_core\Window.h"
#include <Windows.h>
#include <windowsx.h>
#include "Ensum_input\Input.h"


#ifdef _DEBUG
#pragma comment(lib, "Ensum_inputD.lib")
#else
#pragma comment(lib, "Ensum_input.lib")
#endif

namespace Ensum
{
	namespace Core
	{

		class ENSUM_CORE_EXPORT WinWindow :
			public Window
		{
		protected:
			WinWindow();

			virtual const void Frame() = 0;
		public:
			virtual ~WinWindow();


			virtual const void Init();
			virtual const void Start();

			Input::Input* GetInput();
		protected:
			HINSTANCE _hInst;
			HWND _hWnd;
			DWORD _style;
			LPWSTR _wndCaption;

			int _windowPosX, _windowPosY;
			bool _running;

			Input::Input* _input;
		};


		//WndProc and ApplicationHandle is used to make sure we can handle windows messages
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	}
}
#endif