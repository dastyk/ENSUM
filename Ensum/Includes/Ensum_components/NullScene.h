#ifndef _NULL_SCENE_H_
#define _NULL_SCENE_H_

#pragma once
#include "Scene.h"

namespace Ensum
{
	namespace Components
	{
		/** A scene that does nothing.
		*
		*/
		class ENSUM_COMPONENTS_EXPORT NullScene :
			public Scene
		{
		public:
			NullScene(EntityManager& entityManger, Input::Input* input);
			~NullScene();

			/** Frame does nothing.
			*
			*/
			const void Frame()const;
		};

	}
}
#endif