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
			virtual const void _Frame();
			/** Handle messages.
			*
			*/
			virtual const void _MessageHandling();
			/** Called when options have changed.
			*
			*/
			virtual const void _Resize();
		public:
			WinWindow(Delegate<const void()>& sceneManagerFrame);
			virtual ~WinWindow();

			/** Initialization for the window.
			*
			*/
			virtual const void Init();

			/** Returns the HWND instance.
			*
			*/
			const HWND GetHwnd()const;

			/** Static func, for getting the hwnd.
			*
			*/
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