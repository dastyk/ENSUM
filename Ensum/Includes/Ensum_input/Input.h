#ifndef _INPUT_H_
#define _INPUT_H_

#pragma once
#include "dll_export.h"
#include <Windows.h>
#include <unordered_map>
#include <windowsx.h>

namespace Ensum
{
	namespace Input
	{
		static const uint8_t NUM_KEYS = (uint8_t)~0U;
		enum class Keys : uint8_t
		{
			None = 0,

			Back = 0x8,
			Tab = 0x9,

			Enter = 0xd,

			Pause = 0x13,
			CapsLock = 0x14,
			Kana = 0x15,

			Kanji = 0x19,

			Escape = 0x1b,
			ImeConvert = 0x1c,
			ImeNoConvert = 0x1d,

			Space = 0x20,
			PageUp = 0x21,
			PageDown = 0x22,
			End = 0x23,
			Home = 0x24,
			Left = 0x25,
			Up = 0x26,
			Right = 0x27,
			Down = 0x28,
			Select = 0x29,
			Print = 0x2a,
			Execute = 0x2b,
			PrintScreen = 0x2c,
			Insert = 0x2d,
			Delete = 0x2e,
			Help = 0x2f,
			D0 = 0x30,
			D1 = 0x31,
			D2 = 0x32,
			D3 = 0x33,
			D4 = 0x34,
			D5 = 0x35,
			D6 = 0x36,
			D7 = 0x37,
			D8 = 0x38,
			D9 = 0x39,

			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4a,
			K = 0x4b,
			L = 0x4c,
			M = 0x4d,
			N = 0x4e,
			O = 0x4f,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5a,
			LeftWindows = 0x5b,
			RightWindows = 0x5c,
			Apps = 0x5d,

			Sleep = 0x5f,
			NumPad0 = 0x60,
			NumPad1 = 0x61,
			NumPad2 = 0x62,
			NumPad3 = 0x63,
			NumPad4 = 0x64,
			NumPad5 = 0x65,
			NumPad6 = 0x66,
			NumPad7 = 0x67,
			NumPad8 = 0x68,
			NumPad9 = 0x69,
			Multiply = 0x6a,
			Add = 0x6b,
			Separator = 0x6c,
			Subtract = 0x6d,

			Decimal = 0x6e,
			Divide = 0x6f,
			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7a,
			F12 = 0x7b,
			F13 = 0x7c,
			F14 = 0x7d,
			F15 = 0x7e,
			F16 = 0x7f,
			F17 = 0x80,
			F18 = 0x81,
			F19 = 0x82,
			F20 = 0x83,
			F21 = 0x84,
			F22 = 0x85,
			F23 = 0x86,
			F24 = 0x87,

			NumLock = 0x90,
			Scroll = 0x91,

			LeftShift = 0xa0,
			RightShift = 0xa1,
			LeftControl = 0xa2,
			RightControl = 0xa3,
			LeftAlt = 0xa4,
			RightAlt = 0xa5,
			BrowserBack = 0xa6,
			BrowserForward = 0xa7,
			BrowserRefresh = 0xa8,
			BrowserStop = 0xa9,
			BrowserSearch = 0xaa,
			BrowserFavorites = 0xab,
			BrowserHome = 0xac,
			VolumeMute = 0xad,
			VolumeDown = 0xae,
			VolumeUp = 0xaf,
			MediaNextTrack = 0xb0,
			MediaPreviousTrack = 0xb1,
			MediaStop = 0xb2,
			MediaPlayPause = 0xb3,
			LaunchMail = 0xb4,
			SelectMedia = 0xb5,
			LaunchApplication1 = 0xb6,
			LaunchApplication2 = 0xb7,

			OemSemicolon = 0xba,
			OemPlus = 0xbb,
			OemComma = 0xbc,
			OemMinus = 0xbd,
			OemPeriod = 0xbe,
			OemQuestion = 0xbf,
			OemTilde = 0xc0,

			OemOpenBrackets = 0xdb,
			OemPipe = 0xdc,
			OemCloseBrackets = 0xdd,
			OemQuotes = 0xde,
			Oem8 = 0xdf,

			OemBackslash = 0xe2,

			ProcessKey = 0xe5,

			OemCopy = 0xf2,
			OemAuto = 0xf3,
			OemEnlW = 0xf4,

			Attn = 0xf6,
			Crsel = 0xf7,
			Exsel = 0xf8,
			EraseEof = 0xf9,
			Play = 0xfa,
			Zoom = 0xfb,

			Pa1 = 0xfd,
			OemClear = 0xfe,
		};

		static const uint8_t NUM_MOUSEKEYS = (uint8_t)32U;
		enum class MouseKeys : uint8_t
		{
			None = 0x0,

			LButton = 0x1,
			MButton = 0x2,
			RButton = 0x3,

			XButton1 = 0x4,
			XButton2 = 0x5,
		};
		class ENSUM_INPUT_EXPORT Input
		{
		public:


			Input();
			~Input();


			const void Frame();
	

			const bool IsKeyDown(Keys keyCode)const;
			const bool IsKeyPushed(Keys keyCode);

			const bool IsScrollDown(int32_t&delta);
			const bool IsScrollUp(int32_t& delta);
			
			const bool IsMouseKeyDown(MouseKeys keyCode)const;
			const bool IsMouseKeyPushed(MouseKeys keyCode);

			const void GetMousePos(int32_t& rX, int32_t& rY)const;
			const void GetMouseDiff(int32_t& rX, int32_t& rY)const;

			const void LockMouseToCenter(bool lock);
			const void LockMouseToWindow(bool lock);
			const void HideCursor(bool show)const;

			const void Rebind(Keys org, Keys to);
			const void Rebind(MouseKeys org, MouseKeys to);

			//const void Rebind(Keys org, MouseKeys to);
			//const void Rebind(MouseKeys org, Keys to);
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && defined(WM_USER)
			const void Init(HWND hwnd);
			LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
#endif
		private:
			const void _KeyDown(Keys keyCode);
			const void _KeyUp(Keys keyCode);

			const void _MouseDown(MouseKeys keyCode);
			const void _MouseUp(MouseKeys keyCode);
			const void _OnMouseMove(uint32_t x, uint32_t y);
			const void _OnMouseScroll(int32_t delta);

		private:
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && defined(WM_USER)
			HWND _hwnd;
#endif
			bool _keys[NUM_KEYS];
			bool _keyPressed[NUM_KEYS];

			bool _mouseKeys[NUM_MOUSEKEYS];
			bool _mouseKeyPressed[NUM_MOUSEKEYS];

			int32_t _mousePosX, _mousePosY, _lastMousePosX, _lastMousePosY, _xDiff, _yDiff;

			bool _mouseLockedToScreen;
			bool _mouseLockedToCenter;
			int32_t _scrollDelta;

			std::unordered_map<Keys, Keys>* _keyToKey;
			std::unordered_map<MouseKeys, MouseKeys>* _mousekeyToMousekey;
		};
	}
}
#endif