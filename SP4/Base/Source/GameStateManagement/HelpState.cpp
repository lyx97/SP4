#include "HelpState.h"
#include "GL/glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"
#include "MouseController.h"

#include "KeyboardController.h"
#include "SceneManager.h"

CHelpState::CHelpState()
{
}

CHelpState::~CHelpState()
{
}

void CHelpState::Init()
{
	//Camera Space View
	m_orthoHeight = 200;
	m_orthoWidth = m_orthoHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();

	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->Init();

	sprites[0] = Create::Sprite2DObject("background", Vector3(0, 0, 0));
	sprites[1] = Create::Sprite2DObject("treasure_texture", Vector3(0, 0, 0));
	sprites[2] = Create::Sprite2DObject("powerup_maxhealth", Vector3(0, 0, 0));
	sprites[3] = Create::Sprite2DObject("powerup_healthrecover", Vector3(0, 0, 0));
	sprites[4] = Create::Sprite2DObject("powerup_healthregen", Vector3(0, 0, 0));
	sprites[5] = Create::Sprite2DObject("powerup_speed", Vector3(0, 0, 0));
	sprites[6] = Create::Sprite2DObject("healthicon", Vector3(0, 0, 0));

	sprites[7] = Create::Sprite2DObject("wall", Vector3(0, 0, 0));
	sprites[8] = Create::Sprite2DObject("floor", Vector3(0, 0, 0));
	sprites[9] = Create::Sprite2DObject("obstacle", Vector3(0, 0, 0));
	sprites[10] = Create::Sprite2DObject("doorlocked", Vector3(0, 0, 0));
	sprites[11] = Create::Sprite2DObject("doorlocked2", Vector3(0, 0, 0));
	sprites[12] = Create::Sprite2DObject("stair", Vector3(0, 0, 0));

	for (int i = 0; i < 6; ++i)
		text[i] = Create::Text2DObject("text", Vector3(0, 0, 0), "", Vector3(75, 75, 75), Color(1, 1, 1));
}

void CHelpState::Update(double dt)
{
	{//handles required mouse calculations
		MousePosX = Application::GetInstance().GetWorldBasedMousePos().x;
		MousePosY = Application::GetInstance().GetWorldBasedMousePos().z;
		float w = Application::GetInstance().GetWindowWidth();
		float h = Application::GetInstance().GetWindowHeight();
		MousePosX = m_orthoWidth * (MousePosX / w) * 1.4f;
		MousePosY = m_orthoHeight * (MousePosY / h) * 2;
	}
	sprites[0]->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.5f, 0.0f));
	sprites[0]->SetScale(Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));
	
	for (int i = 1; i < 6; ++i)
	{
		sprites[i]->SetPosition(Vector3(i * 100, Application::GetInstance().GetWindowHeight() * 0.6f, 1.0f));
		sprites[i]->SetScale(Vector3(75, 75, 1.0f));
	}
	for (int i = 7; i < 13; ++i)
	{
		sprites[i]->SetPosition(Vector3((i - 6) * 100, Application::GetInstance().GetWindowHeight() * 0.4f, 1.0f));
		sprites[i]->SetScale(Vector3(75, 75, 1.0f));
	}
	std::ostringstream ss;
	ss << "These are powerups and treasures that aid you.";
	text[0]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.7f, 1.0f));
	text[0]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowHeight() * 0.04f, 50));
	text[0]->SetText(ss.str());

	ss.str("");
	ss << "Backspace to return to main menu";
	text[1]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.9f, 1.0f));
	text[1]->SetScale(Vector3(50, 50, 50));
	text[1]->SetText(ss.str());

	ss.str("");
	ss << "These are the tiles, enter the game to find out what it does!";
	text[2]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.5f, 1.0f));
	text[2]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowHeight() * 0.04f, 50));
	text[2]->SetText(ss.str());

	ss.str("");
	ss << "Use WASD to move, Mouse to aim, LMB to shoot, RMB to dash";
	text[3]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.3f, 1.0f));
	text[3]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowHeight() * 0.04f, 50));
	text[3]->SetText(ss.str());

	ss.str("");
	ss << "Q to pickup treasures (first icon in the top row)";
	text[4]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
	text[4]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowHeight() * 0.04f, 50));
	text[4]->SetText(ss.str());

	ss.str("");
	ss << "E to use treasures, U to toggle between simple and complex UI";
	text[5]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.1f, 1.0f));
	text[5]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowHeight() * 0.04f, 50));
	text[5]->SetText(ss.str());

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
		SceneManager::GetInstance()->SetActiveScene("MenuState");
}

void CHelpState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	//EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection
		(
		0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10,
		10
		);

	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}

void CHelpState::Exit()
{
	// Remove the enity from EntityManager
	for (auto q : sprites)
		EntityManager::GetInstance()->RemoveEntity(q);
	for (auto w : text)
		EntityManager::GetInstance()->RemoveEntity(w);

	MeshBuilder::GetInstance()->Destroy();

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}