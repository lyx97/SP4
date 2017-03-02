#include <iostream>
using namespace std;

#include "MenuState.h"
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
#include "../Sound/SoundManager.h"

#include "SceneManager.h"

CMenuState::CMenuState()
{
}

CMenuState::~CMenuState()
{
}

void CMenuState::Init()
{
	//Camera Space View
	m_orthoHeight = 200;
	m_orthoWidth = m_orthoHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();

	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//UI//background.tga");
	MeshBuilder::GetInstance()->GenerateQuad("menutitle", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("menutitle")->textureID = LoadTGA("Image//UI//title.tga");
	MeshBuilder::GetInstance()->GenerateQuad("startgame", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("startgame")->textureID = LoadTGA("Image//UI//startgame.tga");
	MeshBuilder::GetInstance()->GenerateQuad("instructions", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("instructions")->textureID = LoadTGA("Image//UI//instructions.tga");
	MeshBuilder::GetInstance()->GenerateQuad("quit", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quit")->textureID = LoadTGA("Image//UI//quit.tga");

	sprites[0] = Create::Sprite2DObject("MENUSTATE_BKGROUND", Vector3(0, 0, 0));
	sprites[1] = Create::Sprite2DObject("menutitle", Vector3(0,0,0));
	sprites[2] = Create::Sprite2DObject("startgame", Vector3(0, 0, 0));
	sprites[3] = Create::Sprite2DObject("instructions", Vector3(0, 0, 0));
	sprites[4] = Create::Sprite2DObject("quit", Vector3(0, 0, 0));
}

void CMenuState::Update(double dt)
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

	sprites[1]->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.9f, 1.0f));
	sprites[1]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.7f, Application::GetInstance().GetWindowHeight() * 0.2, 1.0f));

	sprites[2]->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.5f, 1.0f));
	sprites[2]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.3f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));

	sprites[3]->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.3f, 1.0f));
	sprites[3]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.6f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));

	sprites[4]->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.1f, 1.0f));
	sprites[4]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));

	// start game
	if (MousePosX >= -60 && MousePosX <= 60 && MousePosY >= -25 && MousePosY <= 15)
	{
		sprites[2]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.35f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			SoundManager::GetInstance()->PlaySoundEffect2D("Sound//menu_hover.wav");
			SceneManager::GetInstance()->SetActiveScene("GameState");
		}
	}
	else
	{
		sprites[2]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.3f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
	}

	// instructions
	if (MousePosX >= -60 && MousePosX <= 60 && MousePosY >= 60 && MousePosY <= 95)
	{
		sprites[3]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.65f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			SoundManager::GetInstance()->PlaySoundEffect2D("Sound//menu_hover.wav");
			SceneManager::GetInstance()->SetActiveScene("HelpState");
		}
	}
	else
	{
		sprites[3]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.6f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
	}

	// quit
	if (MousePosX >= -60 && MousePosX <= 60 && MousePosY >= 145 && MousePosY <= 175)
	{
		sprites[4]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.15f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			SoundManager::GetInstance()->PlaySoundEffect2D("Sound//menu_hover.wav");
			Application::GetInstance().Exit();
		}
	}
	else
	{ 		
		sprites[4]->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowHeight() * 0.2f, 1.0f));
	}

}

void CMenuState::Render()
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

void CMenuState::Exit()
{
	// Remove the enity from EntityManager
	for (auto q : sprites)
		EntityManager::GetInstance()->RemoveEntity(q);

	MeshBuilder::GetInstance()->Destroy();

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}