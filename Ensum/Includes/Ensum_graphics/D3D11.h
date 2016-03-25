#ifndef _D3D11_H_
#define _D3D11_H_
#pragma once
#include "Ensum_graphics\Graphics.h"
#include <Windows.h>

namespace Ensum
{
	namespace Graphics
	{
		class ENSUM_GRAPHICS_EXPORT D3D11 :
			public Graphics
		{
		protected:
			D3D11(HWND hwnd);
	
		public:
			virtual ~D3D11();
			static void CreateInstance(HWND hwnd);
			static void DeleteInstance();

			const void Init();
			const void Shutdown();
		protected:

			const void _Frame();
		};

	}
}
#endif