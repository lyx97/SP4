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
#include "SceneManager.h"

CMenuState::CMenuState()
{
}

CMenuState::~CMenuState()
{
}

void CMenuState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//MenuState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	MenuStateBackground = Create::Sprite2DObject("MENUSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	cout << "MENU state loaded!\n" << endl;
}

void CMenuState::Update(double dt)
{
	MouseController::GetInstance()->GetMousePosition(MousePosX, MousePosY);

	//Play
	if (MousePosX >= 257 && MousePosX < 529 && MousePosY <= 216 && MousePosY >= 153)
	{
		MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//main_play.tga");
	}
	//Option
	else if (MousePosX >= 207 && MousePosX < 586 && MousePosY <= 306 && MousePosY >= 244)
	{
		MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//main_option.tga");
	}
	//Exit
	else if (MousePosX >= 277 && MousePosX < 529 && MousePosY <= 405 && MousePosY >= 340)
	{
		MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//main_exit.tga");
	}

	if (MousePosX >= 257 && MousePosX < 529 && MousePosY <= 216 && MousePosY >= 153 && MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Loading CGameState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	}
	else if (MousePosX >= 207 && MousePosX < 586 && MousePosY <= 306 && MousePosY >= 244 && MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Loading COptionState" << endl;
		SceneManager::GetInstance()->SetActiveScene("OptionState");
	}
	else if (MousePosX >= 277 && MousePosX < 529 && MousePosY <= 405 && MousePosY >= 340 && MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
	}

	cout << MousePosX << ", " << MousePosY << endl;
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
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	// Remove the meshes which are specific to CIntroState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	//Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}