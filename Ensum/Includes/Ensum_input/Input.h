#ifndef _INPUT_H_
#define _INPUT_H_

#pragma once
#include "dll_export.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <unordered_map>

namespace Ensum
{
	namespace Input
	{
		class ENSUM_INPUT_EXPORT Input
		{
		public:
			Input();
			~Input();


			const void Init(HWND hwnd);
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && defined(WM_USER)
			LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
#endif
			bool Escape();
		private:
			std::unique_ptr<DirectX::Keyboard>* _keyboard;
			std::unique_ptr<DirectX::Mouse>* _mouse;

			std::unordered_map<DirectX::Keyboard::Keys, DirectX::Keyboard::Keys>* _keyToKey;
			std::unordered_map<DirectX::Mouse::Keys, DirectX::Mouse::Keys>* _mousekeyToMousekey;
		};
	}
}
#endif