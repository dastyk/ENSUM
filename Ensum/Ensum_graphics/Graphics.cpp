#include "Ensum_graphics\Graphics.h"

namespace Ensum
{
	namespace Graphics
	{
		Graphics* Graphics::_instance = nullptr;



		Graphics::Graphics() : _shutdown(false), _threadHandle(nullptr)
		{
		}


		Graphics::~Graphics()
		{

		}
		const void Graphics::Start()
		{
			while (!_shutdown)
			{
				_Frame();
			}
		}
	}
}