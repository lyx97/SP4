#include <iostream>
using namespace std;

#include "LoseState.h"
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

LoseState::LoseState()
{
}

LoseState::~LoseState()
{
}

void LoseState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->Init();

	sprite = Create::Sprite2DObject("background", Vector3(0, 0, 0));
	text = Create::Text2DObject("text", Vector3(0, 0, 0), "You died.. :(", Vector3(100, 100, 100), Color(1, 1, 1));
	text1 = Create::Text2DObject("text", Vector3(0, 0, 0), "Backspace to return to main menu", Vector3(50, 50, 50), Color(1, 1, 1));
}

void LoseState::Update(double dt)
{
	sprite->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.5f, 0.0f));
	sprite->SetScale(Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));

	text->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.5f, 1.0f));
	text->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.05f, Application::GetInstance().GetWindowHeight() * 0.06f, 50));
	text1->SetPosition(Vector3(0, Application::GetInstance().GetWindowHeight() * 0.3f, 1.0f));
	text1->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.03f, Application::GetInstance().GetWindowHeight() * 0.04f, 50));

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
		SceneManager::GetInstance()->SetActiveScene("MenuState");
}

void LoseState::Render()
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

void LoseState::Exit()
{
	// Remove the enity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(sprite);
	EntityManager::GetInstance()->RemoveEntity(text);
	EntityManager::GetInstance()->RemoveEntity(text1);

	MeshBuilder::GetInstance()->Destroy();

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}