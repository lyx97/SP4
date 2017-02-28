#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "RenderHelper.h"
#include "Level/Level.h"
#include "Particle/Particle.h"
#include "Enemy/FlyingTongue.h"
#include "Enemy/Skull.h"
#include "Enemy/Spitter.h"
#include "Enemy/Reaper.h"
#include "Obstacle/Obstacle.h"

#include <iostream>
using namespace std;

SceneText::SceneText()
{
}

SceneText::~SceneText()
{
    CWaypointManager::GetInstance()->DropInstance();
}

void SceneText::Init()
{
	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	// Create the playerinfo instance, which manages all information about the player
	camera.Init(Vector3(0, 1, 0), Vector3(0, 1, 0), Vector3(0, 0, -1));

	// Create and attach the camera to the scene
	//World Space
	m_worldHeight = 300;
	m_worldWidth = m_worldHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();

	//Camera Space View
	m_orthoHeight = 200;
	m_orthoWidth = m_orthoHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();

	//camera.entityList.push_back(playerInfo->GetInstance()->GetPosition());

	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Init Meshes
	MeshBuilder::GetInstance()->Init();

    //spa1->Init(20, 20, 10, 10, 0, 0);
    ////spa1->SetMesh("GRIDMESH");
    ////CSpatialPartition::GetInstance()->SetCamera(&camera);
    //EntityManager::GetInstance()->SetSpatialPartition(spa1);

    unsigned seed = time(NULL);
    srand(seed);
    cout << "Current Seed: " << seed << endl;

    CLevel::GetInstance()->Init(100);

    // Create the playerinfo instance, which manages all information about the player
    playerInfo = CPlayerInfo::GetInstance();
    playerInfo->SetRoomID(0);
    playerInfo->Init();

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

    // Create a Waypoint inside WaypointManager
    //lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_1");
    //int aWayPoint = CWaypointManager::GetInstance()->AddWaypoint(
    //    Vector3(
    //    CLuaInterface::GetInstance()->GetField("x"),
    //    CLuaInterface::GetInstance()->GetField("y"),
    //    CLuaInterface::GetInstance()->GetField("z")
    //    ));

    //lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_2");
    //int anotherWayPoint = CWaypointManager::GetInstance()->AddWaypoint(
    //    aWayPoint,
    //    Vector3(
    //    CLuaInterface::GetInstance()->GetField("x"),
    //    CLuaInterface::GetInstance()->GetField("y"),
    //    CLuaInterface::GetInstance()->GetField("z")
    //    ));

    //lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_3");
    //CWaypointManager::GetInstance()->AddWaypoint(
    //    anotherWayPoint,
    //    Vector3(
    //    CLuaInterface::GetInstance()->GetField("x"),
    //    CLuaInterface::GetInstance()->GetField("y"),
    //    CLuaInterface::GetInstance()->GetField("z")
    //    ));

    //CWaypointManager::GetInstance()->PrintSelf();

    // Create a CEnemy instance
    //enemy2D = new Enemy2D();
    //enemy2D->Init();

	//Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

    minimap = new CMinimap();

    //for (int i = 0; i < 800; ++i)
    //{
    //    Particle* particle = EntityManager::GetInstance()->GetParticle();
    //    particle->Init();
    //    particle->SetIsDone(false);
    //    particle->AddEffect(Particle::GRAVITY);
    //    //particle->AddEffect(Particle::SHRINK);
    //}

    //test = 0.0;

    //FlyingTongue* tongy = new FlyingTongue(0);

    //Skull* skull = new Skull(0);

    //Spitter* spitter = new Spitter(0);

    //Obstacle* obs = new Obstacle(0);

    //Reaper* reaper = new Reaper(0);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}

	cout << "GAME state loaded!\n" << endl;
}

void SceneText::Update(double dt)
{
	{//handles required mouse calculations
		float x, z;
		x = Application::GetInstance().GetWorldBasedMousePos().x;
		z = Application::GetInstance().GetWorldBasedMousePos().z;
		float w = Application::GetInstance().GetWindowWidth();
		float h = Application::GetInstance().GetWindowHeight();
		x = m_orthoWidth * (x / w) * 1.4f;
		//z = -(m_orthoHeight * (h - z) / h);
		z = m_orthoHeight * (z / h) * 2;

		mousePos_screenBased.Set(x, 0, z);
		mousePos_worldBased.Set(
			x + camera.GetCameraTarget().x,
			camera.GetCameraTarget().y,
			z + camera.GetCameraTarget().z);
	}

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('P'))
	{
		GamePaused = !GamePaused;
	}

    //test += dt;

    //int halfWindowWidth = Application::GetInstance().GetWindowWidth() >> 2;
    //int halfWindowHeight = Application::GetInstance().GetWindowHeight() >> 2;

    //if (test >= 10.0)
    //{
    //    for (int i = 0; i < 300; ++i)
    //    {
    //        Particle* particle = EntityManager::GetInstance()->GetParticle();
    //        particle->Init();
    //        particle->SetPosition(
    //            Vector3(Math::RandIntMinMax(-halfWindowWidth, halfWindowWidth),
    //            0,
    //            Math::RandIntMinMax(-halfWindowHeight, halfWindowHeight)));
    //        particle->SetIsDone(false);
    //        particle->AddEffect(Particle::GRAVITY);
    //    }

    //    test = 0.0;
    //}

	if (!GamePaused)
	{
		playerInfo->SetDirection(mousePos_worldBased);
		// Update our entities
		EntityManager::GetInstance()->Update(dt);

		// if the left mouse button was released
		if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB))
		{
			playerInfo->Shoot(mousePos_worldBased);
		}
		if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
		{
			// player's dash, done in player class itself
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
		{
		}
		if (KeyboardController::GetInstance()->IsKeyDown('N'))
		{
			m_orthoHeight--;
			m_orthoWidth = m_orthoHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
		}
		if (KeyboardController::GetInstance()->IsKeyDown('M'))
		{
			m_orthoHeight++;
			m_orthoWidth = m_orthoHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
		}
		if (KeyboardController::GetInstance()->IsKeyPressed('J'))
		{
			Treasure* newTreasure = new Treasure(CPlayerInfo::GetInstance()->GetRoomID());
		}
        if (KeyboardController::GetInstance()->IsKeyDown('L'))
        {
            EntityManager::GetInstance()->GetSpatialPartition(CPlayerInfo::GetInstance()->GetRoomID())->Remove(playerInfo);
            EntityManager::GetInstance()->RemoveSpatialPartition();
            CLevel::GetInstance()->Init(100);
            playerInfo->SetRoomID(0);  
            EntityManager::GetInstance()->GetSpatialPartition(CPlayerInfo::GetInstance()->GetRoomID())->Add(playerInfo);
        }
		// <THERE>
		camera.Update(dt);
		camera.Constrain(playerInfo, 50.0f);

		// Update the player position and other details based on keyboard and mouse inputs
		//playerInfo->Update(dt);

		GraphicsManager::GetInstance()->UpdateLights(dt);
	}
	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	double fps = (1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 2D pipeline then render 2D
    int halfWindowWidth = Application::GetInstance().GetWindowWidth() >> 1;
    int halfWindowHeight = Application::GetInstance().GetWindowHeight() >> 1;

	GraphicsManager::GetInstance()->SetOrthographicProjection(-m_orthoWidth * 0.5f, m_orthoWidth * 0.5f, -m_orthoHeight * 0.5f, m_orthoHeight * 0.5f, -1000, 10000);

	GraphicsManager::GetInstance()->GetViewMatrix().SetToLookAt(
		camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z,
		camera.GetCameraTarget().x, camera.GetCameraTarget().y, camera.GetCameraTarget().z,
		camera.GetCameraUp().x, camera.GetCameraUp().y, camera.GetCameraUp().z);

	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);

	GraphicsManager::GetInstance()->AttachCamera(&camera);
    CLevel::GetInstance()->Render();
	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	GraphicsManager::GetInstance()->GetModelStack().Translate(
		mousePos_worldBased.x,
		mousePos_worldBased.y,
		mousePos_worldBased.z);
	GraphicsManager::GetInstance()->GetModelStack().Scale(10, 10, 10);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("crosshair"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();

	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10000);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}