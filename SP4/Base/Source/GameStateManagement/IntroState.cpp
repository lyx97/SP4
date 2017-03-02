#include <iostream>
using namespace std;

#include "IntroState.h"
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

CIntroState::CIntroState()
{
}

CIntroState::~CIntroState()
{
}

void CIntroState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->Init();

	IntroStateBackground = Create::Sprite2DObject("background", Vector3(0, 0, 0));
	title = Create::Sprite2DObject("menutitle", Vector3(0, 0, 0));
	moveon = Create::Sprite2DObject("intro", Vector3(0, 0, 0));	
	scale = 1.f;

	SoundManager::GetInstance()->PlayBGM("Sound//normal.mp3");
}

void CIntroState::Update(double dt)
{
	IntroStateBackground->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.5f, 0.0f));
	IntroStateBackground->SetScale(Vector3(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight(), 0.0f));

	title->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.9f, 1.0f));
	title->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.7f, Application::GetInstance().GetWindowHeight() * 0.2, 1.0f));

	if (scale >= 1.3f)
		gobig = false;
	else if (scale <= 1.f)
		gobig = true;

	if (gobig)
		scale += dt * 0.5f;
	else
		scale -= dt * 0.5f;

	moveon->SetPosition(Vector3(Application::GetInstance().GetWindowWidth() * 0.5f, Application::GetInstance().GetWindowHeight() * 0.5f, 1.0f));
	moveon->SetScale(Vector3(Application::GetInstance().GetWindowWidth() * 0.7f * scale, 100 * scale, 1.0f));

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		SceneManager::GetInstance()->SetActiveScene("MenuState");
}

void CIntroState::Render()
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

void CIntroState::Exit()
{
	// Remove the enity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);
	EntityManager::GetInstance()->RemoveEntity(title);
	EntityManager::GetInstance()->RemoveEntity(moveon);

	MeshBuilder::GetInstance()->RemoveMesh("background");
	MeshBuilder::GetInstance()->RemoveMesh("title");
	MeshBuilder::GetInstance()->RemoveMesh("intro");

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}