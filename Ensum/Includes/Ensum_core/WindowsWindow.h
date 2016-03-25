#ifndef _WINDOWS_WINDOW_H_
#define _WINDOWS_WINDOW_H_
#pragma once
#include "Ensum_core\Window.h"
#include <Windows.h>
#include <windowsx.h>

namespace Ensum
{
	namespace Core
	{
		/** Windows specific window.
		*
		*/
		class ENSUM_CORE_EXPORT WinWindow :
			public Window
		{
		protected:

			/** The frame function.
			* Put the gamelogic here.
			*/
			virtual const void Frame();
		public:
			WinWindow(Components::SceneManager& sceneManager);
			virtual ~WinWindow();

			/** Initialization for the window.
			*
			*/
			virtual const void Init();
			/** Start the message loop.
			* 
			*/
			virtual const void Start();

			const HWND GetHwnd()const;

			static const HWND GetWindow();
		protected:
			HINSTANCE _hInst;
			HWND _hWnd;
			DWORD _style;

		};


		/**WndProc is used to make sure we can handle windows messages.
		*
		*/
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	}
}
#endif