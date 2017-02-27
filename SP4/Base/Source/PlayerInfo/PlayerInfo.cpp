#include "PlayerInfo.h"
#include <iostream>
#include <sstream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/LaserBlaster.h"
#include "../Lua/LuaInterface.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../Level/Level.h"

// Allocating and initializing CPlayerInfo's static data member.
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: GenericEntity(NULL)
    , prevIndex(Vector3(0, 0, 0))
	, m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(10.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, attachedCamera(NULL)
	, primaryWeapon(NULL)
	, secondaryWeapon(NULL)
	, keyMoveForward('W')
	, keyMoveBackward('S')
	, keyMoveLeft('A')
	, keyMoveRight('D')
	, isMoving(false)
	, isDashed(false)
	, usingTreasure(false)
	, maxHealth(100.0f)
	, health(90.0f)
	, maxSpeed(300.0f)
	, dashDistance(50.0f)
	, dashCooldown(2.f)
	, dashCooldownTimer(0)
	, healthregen(2.5f)
	, healthregenCooldownTimer(0)
	, treasureDurationTimer(0)
	, defaultHealthRegenCooldown(5)
	, defaultSpeed(300.f)
	, dreamBar(MAX_DREAMBAR * 0.5f)
	, killCount(0)
    , rotateLeftLeg(Vector3(0, 0, 0))
    , rotateRightLeg(Vector3(0, 0, 0))
    , rotateLLUP(false)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
	if (secondaryWeapon)
	{
		delete secondaryWeapon;
		secondaryWeapon = NULL;
	}
	if (primaryWeapon)
	{
		delete primaryWeapon;
		primaryWeapon = NULL;
	}
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	this->scale.Set(3, 3, 3);
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	//position.Set(0, 0, 10);
	position.x = CLuaInterface::GetInstance()->getIntValue("PlayerPos_x");
	position.z = CLuaInterface::GetInstance()->getIntValue("PlayerPos_z");
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
    direction.Set(0, 0, 0);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set the pistol as the primary weapon
	primaryWeapon = new CPistol();
	primaryWeapon->Init();
    // Set the laser blaster as the secondary weapon
    secondaryWeapon = new CLaserBlaster();
    secondaryWeapon->Init();
    //secondaryWeapon = new CGrenadeThrow();
    //secondaryWeapon->Init();

	treasure = new Treasure();
	healthregenCooldown = defaultHealthRegenCooldown;
	maxSpeed = defaultSpeed;

    // Initialise the custom keyboard inputs
    keyMoveForward = CLuaInterface::GetInstance()->getCharValue("moveForward");
    keyMoveBackward = CLuaInterface::GetInstance()->getCharValue("moveBackward");
    keyMoveLeft = CLuaInterface::GetInstance()->getCharValue("moveLeft");
    keyMoveRight = CLuaInterface::GetInstance()->getCharValue("moveRight");

    //float distanceSquare = CLuaInterface::GetInstance()->getDistanceSquareValue("CalculateDistanceSquare", Vector3(0, 0, 0), Vector3(10, 10, 10));

    //int a = 1, b = 2, c = 3, d = 4;
    //CLuaInterface::GetInstance()->getVariableValues("GetMinMax", a, b, c, d);


    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Init heatmap
    int xSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax();
    int zSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax();

    heatmap = new CHeatmap*[xSize];
    for (int x = 0; x <= xSize; ++x)
        heatmap[x] = new CHeatmap[zSize + 1];

    // Generate
    CGenerateHeatmap::GetInstance()->GenerateHeatmap(heatmap, xSize, zSize, index.x, index.z);
    CGenerateHeatmap::GetInstance()->CalculateDirection(heatmap, xSize, zSize);

    int xIndex = index.x;
    int zIndex = index.z;

    prevIndex = index;

    // Spawn location -- Fixed to room size of 19
    SpawnLocation[0] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize - 1, zSize >> 1);
    SpawnLocation[1] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(1, zSize >> 1);
    SpawnLocation[2] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize >> 1, zSize - 1);
    SpawnLocation[3] = CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize >> 1, 1);

    // Boundary
    minBoundary.Set(CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(1, 1) - Vector3(GRIDSIZE, 0, GRIDSIZE));
    maxBoundary.Set(CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridPos(xSize - 1, zSize - 1) + Vector3(GRIDSIZE, 0, GRIDSIZE));

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() * 0.5f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() * 0.5f;

	for (int i = 0; i < 4; ++i)
	{
		textOBJ[i] = Create::Text2DObject(
			"text",
			Vector3(-Application::GetInstance().GetWindowWidth() * 0.5f, (Application::GetInstance().GetWindowHeight() * 0.5f) -fontSize* 0.5 - (fontSize * i), 0.0f),
			"",
			Vector3(fontSize, fontSize, fontSize),
			Color(0.0f, 1.0f, 0.0f));
	}
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}

// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

/********************************************************************************
Player Update
********************************************************************************/
void CPlayerInfo::Update(double dt)
{
    int xSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax();
    int zSize = CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax();

    if (prevIndex != index)
    {
        CGenerateHeatmap::GetInstance()->GenerateHeatmap(heatmap, xSize, zSize, index.x, index.z);
        CGenerateHeatmap::GetInstance()->CalculateDirection(heatmap, xSize, zSize);
        prevIndex = index;
    }

	//if (playerMesh)
	//{
	//	playerMesh->m_anim->animActive = true;
	//	playerMesh->Update(dt * 1.5f);
	//}
	if (dreamBar > Math::EPSILON)
		this->dreamBar -= 0.1 * dt;

	position += velocity * (float)dt;
	if (!Application::GetInstance().GetWorldBasedMousePos().IsZero())
		front.Set(Vector3(position - Application::GetInstance().GetWorldBasedMousePos()).Normalized());
	Vector3 forceDir;
	isMoving = false;
	if (!isMoving)
	{
		velocity *= 0.9f;
	}

    if (CLevel::GetInstance()->GetRoom(roomID)->GetRoomCleared() &&
        CLevel::GetInstance()->GetRoom(roomID)->GetSpatialPartition()->GetGridType(index.x, index.z) == GRID_TYPE::DOOR)
    {
        int previousRoom = roomID;

        if (CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMin() == index.x && (CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax() >> 1) == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(0);
            position = SpawnLocation[0];
        }
        if (CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax() == index.x && (CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax() >> 1) == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(1);
            position = SpawnLocation[1];
        }
        if ((CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax() >> 1) == index.x && CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMin() == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(2);
            position = SpawnLocation[2];
        }
        if ((CLevel::GetInstance()->GetRoom(roomID)->GetRoomXMax() >> 1) == index.x && CLevel::GetInstance()->GetRoom(roomID)->GetRoomZMax() == index.z)
        {
            roomID = CLevel::GetInstance()->GetRoom(roomID)->GetDoorToRoomID(3);
            position = SpawnLocation[3];
        }

        EntityManager::GetInstance()->GetSpatialPartition(roomID)->Add(this);
        EntityManager::GetInstance()->GetSpatialPartition(previousRoom)->Remove(this);
    }

    //if (CLevel::GetInstance()->GetLevelChanged())
    //{
    //    int previousRoom = m_iCurrentRoom;

    //    EntityManager::GetInstance()->GetSpatialPartition(m_iCurrentRoom)->Add(this);
    //    EntityManager::GetInstance()->GetSpatialPartition(previousRoom)->Remove(this);

    //    CLevel::GetInstance()->SetLevelChanged(false);
    //}

    // Dash cooldown
	if (isDashed)
	{
		dashCooldownTimer -= dt;
		if (dashCooldownTimer <= 0)
		{
			isDashed = false;
		}
	}
	// Health regeneration cooldown
	healthregenCooldownTimer -= dt;
	if (healthregenCooldownTimer <= 0)
	{
		if (health + healthregen > maxHealth)
			health = maxHealth;
		else
			health += healthregen;

		healthregenCooldownTimer = healthregenCooldown;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward))
	{
        forceDir.z -= 1;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
	{
        forceDir.z += 1;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
	{
        forceDir.x -= 1;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
        forceDir.x += 1;
	}

	if (velocity.LengthSquared() < maxSpeed * maxSpeed)
	{
		isMoving = true;
		forceMagnitude = maxSpeed;
		this->ApplyForce(forceDir, forceMagnitude * dt);
	}

	if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (!forceDir.IsZero() && !isDashed)
		{
			forceMagnitude = maxSpeed * dashDistance;
			this->ApplyForce(forceDir, forceMagnitude * dt);
			isDashed = true;
			dashCooldownTimer = dashCooldown;
		}
	}

	for (auto entity : EntityManager::GetInstance()->GetEntityList())
	{
		if (entity->GetEntityType() == TREASURE)
		{
			Treasure* newTreasure = dynamic_cast<Treasure*>(entity);
			if ((newTreasure->GetPosition() - this->position).LengthSquared() < 100 &&
				KeyboardController::GetInstance()->IsKeyPressed('Q'))
			{
				if (treasure->treasure_type == Treasure::NONE)
				{
					AddTreasures(newTreasure->random);
					newTreasure->SetIsDone(true);
				}
				else
				{
					int tempType = this->treasure->treasure_type;
					AddTreasures(newTreasure->random);
					newTreasure->SetIsDone(true);
					Treasure* spawnTreasure = new Treasure();
					spawnTreasure->SpawnTreasure(this->position, tempType);
				}
			}
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		if (this->killCount >= treasure->GetCooldown())
		{
			treasureDurationTimer = 0;
			usingTreasure = true;
			this->killCount = 0;
		}
		else
		{
			cout << "NOT ENOUGH KILLS" << endl;
		}
	}
	if (usingTreasure)
	{
		treasure->Effects();
		treasureDurationTimer += dt;
		if (treasureDurationTimer >= treasure->GetDuration())
		{
			usingTreasure = false;
			treasureDurationTimer = 0;
			// set back to default values
			Revert();
		}
	}

	Constrain();
	if (primaryWeapon)
		primaryWeapon->Update(dt);
	if (secondaryWeapon)
		secondaryWeapon->Update(dt);

	// If the user presses R key, then reset the view to default values
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset();
	}
	else
	{
		//UpdateJumpUpwards(dt);
		//UpdateFreeFall(dt);
	}

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		//attachedCamera->SetCameraPos(position);
		//attachedCamera->SetCameraTarget(target);
		//attachedCamera->SetCameraUp(up);
	}

	if (rotateLeftLeg.z <= -10)
    {
        rotateLLUP = true;
    }
    else if (rotateLeftLeg.z >= 10)
    {
        rotateLLUP = false;
    }

    if (rotateLLUP)
    {
        rotateLeftLeg.z += dt * 100;
        rotateRightLeg.z -= dt * 100;
    }
    else if (!rotateLLUP)
    {
        rotateLeftLeg.z -= dt * 100;
        rotateRightLeg.z += dt * 100;
    }

	// DEBUGGING TOOLS
	if (KeyboardController::GetInstance()->IsKeyPressed('O'))
	{
		this->health -= 10;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('I'))
	{
		this->dreamBar -= 10;
	}
}

void CPlayerInfo::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelStack.PushMatrix();
	modelStack.Translate(this->position.x, this->position.y, this->position.z);
    modelStack.Rotate(Math::RadianToDegree(atan2(direction.z, direction.x)), 0, -1, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("body"));

    modelStack.PushMatrix();
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("head"));
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Rotate(rotateLeftLeg.z, 0, 0, -1);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("leftleg"));
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Rotate(rotateRightLeg.z, 0, 0, -1);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rightleg"));
    modelStack.PopMatrix();

    modelStack.PopMatrix();

    //for (int x = 0; x <= CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMax(); ++x)
    //{
    //    for (int z = 0; z <= CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMax(); ++z)
    //    {
    //        GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
    //        Vector3 temp = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(x, z);
    //        Vector3 dir = heatmap[x][z].GetDir();
    //        GraphicsManager::GetInstance()->GetModelStack().Translate(temp.x, 1, temp.z);
    //        GraphicsManager::GetInstance()->GetModelStack().Rotate(-90, 1, 0, 0);
    //        GraphicsManager::GetInstance()->GetModelStack().Rotate(Math::RadianToDegree(atan2(dir.x, dir.z)), 0, 0, 1);
    //        GraphicsManager::GetInstance()->GetModelStack().Scale(scale.x, scale.y, scale.z);
    //        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("direction"));
    //        GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
    //    }
    //}
}

void CPlayerInfo::RenderUI()
{
	for (int i = 0; i < 4; ++i)
	{
		textOBJ[i]->SetPosition(Vector3(
			-Application::GetInstance().GetWindowWidth() * 0.5f + 1, 
			(Application::GetInstance().GetWindowHeight() * 0.5f) - (fontSize * i) - fontSize * 0.5f,
			1.0f));
	}
	std::ostringstream ss;
	ss.precision(3);
	ss << "  Health: " << endl;
	textOBJ[0]->SetText(ss.str());
	ss.str("");
	ss << "    Dash: " << endl;
	textOBJ[1]->SetText(ss.str());
	ss.str("");
	ss << "   Speed: " << this->maxSpeed << endl;
	textOBJ[2]->SetText(ss.str());
	ss.str("");
	ss << "Dreambar: " << this->dreamBar << endl;
	textOBJ[3]->SetText(ss.str());

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	// HEALTH
	modelStack.PushMatrix();
	modelStack.Translate(
		-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 7.0f),
		(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 0.5f,
		1);
	modelStack.Translate(this->health - fontSize * 2, 0, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale(this->health, fontSize * 0.5f, 1);
	float healthRatio = this->health / this->maxHealth;
	if (healthRatio < 0.25f)
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_quad"));
	else if (healthRatio < 0.5f)
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_half"));
	else
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_full"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(
		-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 7.0f),
		(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 0.5f,
		0.5f);
	modelStack.Translate(this->maxHealth - fontSize * 2, 0, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale(this->maxHealth, fontSize * 0.5f, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
	modelStack.PopMatrix();

	// DASH
	modelStack.PushMatrix();
	modelStack.Translate(
		-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 7.0f),
		(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 1.5f,
		1);
	modelStack.Translate(((this->dashCooldown - this->dashCooldownTimer) * 10 * this->dashCooldown) - fontSize * 2, 0, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale((this->dashCooldown - this->dashCooldownTimer) * 10 * this->dashCooldown, fontSize * 0.5f, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dash"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(
		-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 7.0f),
		(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 1.5f,
		0.5f);
	modelStack.Translate((this->dashCooldown * 10 * this->dashCooldown) - fontSize * 2, 0, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale(this->dashCooldown * 10 * this->dashCooldown, fontSize * 0.5f, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
	modelStack.PopMatrix();

	// DREAMBAR
	modelStack.PushMatrix();
	modelStack.Translate(
		-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 7.0f),
		(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 3.5f,
		1);
	modelStack.Translate(this->dreamBar - fontSize * 2, 0, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale(this->dreamBar, fontSize * 0.5f, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dreambar"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(
		-Application::GetInstance().GetWindowWidth() * 0.5f + (fontSize * 7.0f),
		(Application::GetInstance().GetWindowHeight() * 0.5f) - fontSize * 3.5f,
		0.5f);
	modelStack.Translate(100 - fontSize * 2, 0, 0);
	modelStack.Scale(2, 2, 2);
	modelStack.Scale(100, fontSize * 0.5f, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("border"));
	modelStack.PopMatrix();
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
    if (position.x > maxBoundary.x - 1.0f)
    {
        position.x = maxBoundary.x - 1.0f; 
        velocity.x = 0;
    }
    if (position.z > maxBoundary.z - 1.0f)
    {
        position.z = maxBoundary.z - 1.0f; 
        velocity.z = 0;
    }
    if (position.x < minBoundary.x + 1.0f)
    {
        position.x = minBoundary.x + 1.0f;
        velocity.x = 0;
    }
    if (position.z < minBoundary.z + 1.0f)
    {
        position.z = minBoundary.z + 1.0f;
        velocity.z = 0;
    }
}

void CPlayerInfo::AttachCamera(Camera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}

void CPlayerInfo::Shoot(Vector3 dir)
{
	if (secondaryWeapon)
		secondaryWeapon->Discharge(this->position, dir, this);
}

void CPlayerInfo::RecoverHealth()
{ 
	if (health + 10 > maxHealth)
		health = maxHealth;
	else
		health += 10;
}

void CPlayerInfo::AddTreasures(int type)
{
	treasure->treasure_type = (Treasure::TREASURES_TYPE)type;
	this->treasure->SetValues();
}

void CPlayerInfo::Revert()
{
	healthregenCooldown = defaultHealthRegenCooldown;
	maxSpeed = defaultSpeed;
}
