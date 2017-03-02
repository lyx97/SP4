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

	text[0] = Create::Text2DObject("text", Vector3(0, 0, 0), "", Vector3(75, 75, 75), Color(1, 1, 1));
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
		sprites[i]->SetPosition(Vector3(i * 100, Application::GetInstance().GetWindowHeight() * 0.7f, 1.0f));
		sprites[i]->SetScale(Vector3(75, 75, 1.0f));
	}
	std::ostringstream ss;
	ss << "These are powerups and treasures that aid you.";
	text[0]->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.8f, 1.0f));
	text[0]->SetText(ss.str());

	//sprites[6]->SetPosition(Vector3(100, Application::GetInstance().GetWindowHeight() * 0.5f, 1.0f));
	//sprites[6]->SetScale(Vector3(75, 75, 1.0f));

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

	MeshBuilder::GetInstance()->RemoveMesh("HELPSTATE_BKGROUND");
	MeshBuilder::GetInstance()->RemoveMesh("treasure_texture"); 

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}