#ifndef _D3D11_H_
#define _D3D11_H_
#pragma once
#include "Ensum_graphics\Graphics.h"
#include "Direct3D11.h"
#include <Windows.h>

namespace Ensum
{
	namespace Graphics
	{
		class ENSUM_GRAPHICS_EXPORT D3D11 :
			public Graphics
		{
		public:
			D3D11();
			virtual ~D3D11();



			/** Create all the graphics stuff.
			*
			*/
			virtual const void OnCreateDevice();
			/** Destroy all the graphics stuff.
			*
			*/
			virtual const void OnDestroyDevice();
		protected:
			/** Options change func.
			*
			*/
			virtual const void _OnOptionsChange();

			/** Begin the frame.
			*
			*/
			virtual const void _BeginFrame(void);
			/** End the frame.
			*
			*/
			virtual const void _EndFrame(void);

			/** Render everything.
			*
			*/
			virtual const void _Frame();

		protected:
			Direct3D11 _D3D11;
		};

	}
}
#endif