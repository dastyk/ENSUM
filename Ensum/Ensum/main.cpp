#include <Windows.h>
#include <windowsx.h>



#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif // DBG_NEW
#endif  // _DEBUG




#include "Ensum_core\WindowsWindow.h"
#include "Ensum_components\SceneManager.h"
#include "Ensum_components\NullScene.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Options.h"
#include "Ensum_components\DataManager.h"
#include "Ensum_graphics\D3D12.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_coreD.lib")
#pragma comment(lib, "Ensum_componentsD.lib")
#pragma comment(lib, "Ensum_utilsD.lib")
#pragma comment(lib, "Ensum_inputD.lib")
#pragma comment(lib, "Ensum_graphicsD.lib")
#else
#pragma comment(lib, "Ensum_core.lib")
#pragma comment(lib, "Ensum_components.lib")
#pragma comment(lib, "Ensum_utils.lib")
#pragma comment(lib, "Ensum_input.lib")
#pragma comment(lib, "Ensum_graphics.lib")
#endif

using namespace Ensum;


class Game : public Components::Scene
{
public:
	Game(Components::EntityManager& entityManger, Input::Input* input, Components::SceneManager& scenemanager):Scene(entityManger,input) ,_sceneManager(scenemanager)
	{
		_input->LockMouseToWindow(true);
		_input->LockMouseToCenter(true);
		_input->HideCursor(true);
		_input->Rebind("Exit", Input::Keys::Escape);
		_sceneManager.CreateScene(new Components::NullScene(_sceneManager.GetEntityManager(), _input));
		//_entityManager.Delete(_entity);

		Components::Entity player = _entityManager.Create();
		_data.CreateData(player);

		_data.AddFloatValue(player, "Health", 100.0f);
		float hp = _data.GetFloatValue(player, "Health", 0.0f);

		//Utils::Options::SetIntegerOption("Screen", "Width", 1337);

		Components::Entity ent = _entityManager.Create();

		_transform.CreateTransform(ent);
		_transform.CreateTransform(player);

		_transform.SetPosition(player, DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f));
		_transform.SetPosition(ent, DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));

		_transform.SetForward(player, DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
		//_transform.SetForward(player, DirectX::XMFLOAT3(-1.0f, 0.0f, 1.0f));

		_transform.BindChild(player, ent);
		_transform.UnbindChild(ent);
	}

	//const void Frame()
	//{
	//	Components::Scene::Frame();
	//	if (_input->IsKeyPushed(Input::Keys::F))
	//	{
	//		Utils::Options::SetBooleanOption("Screen", "Fullscreen", true);
	//		Utils::Options::NotifyChange();
	//	}
	//	if (_input->IsKeyPushed(Input::Keys::G))
	//	{
	//		Utils::Options::SetBooleanOption("Screen", "Fullscreen", false);
	//		Utils::Options::NotifyChange();
	//	}
	//}
	~Game()
	{
	}
private:
	Components::SceneManager& _sceneManager;
};






int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Memoryleak detection.
		//					  _crtBreakAlloc = 409;
#endif
	Utils::ConsoleLog::CreateInstance();
	Utils::Options::CreateInstance();


	Components::SceneManager sceneManager;

	//Utils::Options::SetIntegerOption("Screen", "Width", 1337);


	Core::Window* w = Core::Window::CreateWin(new Core::WinWindow(sceneManager));
	
	Graphics::Graphics::CreateInstance(new Graphics::D3D12());


	sceneManager.CreateScene(new Game(sceneManager.GetEntityManager(), w->GetInput(), sceneManager));
	
	w->Start();

	Graphics::D3D12::DeleteInstance();
	Core::Window::DeleteInstance();
	Utils::Options::DeleteInstance();
	Utils::ConsoleLog::DeleteInstance();

	return 0;
}
