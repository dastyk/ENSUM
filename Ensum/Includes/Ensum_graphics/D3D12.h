#ifndef _D3D12_H_
#define _D3D12_H_

#pragma once
#include "Direct3D12.h"
#include "Ensum_graphics\Graphics.h"

namespace Ensum
{
	namespace Graphics
	{

		class ENSUM_GRAPHICS_EXPORT D3D12 : public Graphics
		{
		public:
			D3D12();
			virtual ~D3D12();

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
			Direct3D12 _D3D12;
		protected:
			D3D12_VIEWPORT _viewPort;
		};
	}
}
#endif