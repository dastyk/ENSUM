#include "Ensum_components\NullScene.h"

namespace Ensum
{
	namespace Components
	{

		NullScene::NullScene(EntityManager& entityManger, Input::Input* input) : Scene(entityManger, input)
		{
		}


		NullScene::~NullScene()
		{
			int i = 0;
		}
	}
}