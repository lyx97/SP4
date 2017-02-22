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
    , m_iCurrentRoom(0)
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
	, maxHealth(100.0f)
	, health(90.0f)
	, maxSpeed(300.0f)
	, healthRegen(0.5f)
	, dashCooldownTimer(0)
	, healthregenCooldownTimer(0)
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
	this->scale.Set(15, 25, 15);
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

	playerMeshes[0] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_down");		// idle
	playerMeshes[1] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_up");		// up
	playerMeshes[2] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_down");		// down
	playerMeshes[3] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_left");		// left
	playerMeshes[4] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_right");	// right
	//playerMeshes[5] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_up");		// shoot up
	//playerMeshes[6] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_up");		// shoot down
	//playerMeshes[7] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_up");		// shoot left
	//playerMeshes[8] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_up");		// shoot right

	for (int i = 0; i < 5; ++i)
	{
		playerMeshes[i]->m_anim = new Animation;
		playerMeshes[i]->m_anim->Set(0, 13, 1, 1.f, true);
	}
	playerMesh = playerMeshes[0];
	spriteAnimation = (SpriteAnimation*)(MeshBuilder::GetInstance()->GetMesh("player_up"));
	if (spriteAnimation)
	{
		spriteAnimation->m_anim = new Animation();
		spriteAnimation->m_anim->Set(0, 13, 1, 1.f, true);
	}

    // Initialise the custom keyboard inputs
    keyMoveForward = CLuaInterface::GetInstance()->getCharValue("moveForward");
    keyMoveBackward = CLuaInterface::GetInstance()->getCharValue("moveBackward");
    keyMoveLeft = CLuaInterface::GetInstance()->getCharValue("moveLeft");
    keyMoveRight = CLuaInterface::GetInstance()->getCharValue("moveRight");

    //float distanceSquare = CLuaInterface::GetInstance()->getDistanceSquareValue("CalculateDistanceSquare", Vector3(0, 0, 0), Vector3(10, 10, 10));

    //int a = 1, b = 2, c = 3, d = 4;
    //CLuaInterface::GetInstance()->getVariableValues("GetMinMax", a, b, c, d);

    EntityManager::GetInstance()->AddEntity(this, m_iCurrentRoom);

    // Init heatmap
    int xSize = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMax();
    int zSize = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMax();

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
    SpawnLocation[0] = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(xSize - 1, zSize >> 1);
    SpawnLocation[1] = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(1, zSize >> 1);
    SpawnLocation[2] = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(xSize >> 1, zSize - 1);
    SpawnLocation[3] = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(xSize >> 1, 1);

    // Boundary
    minBoundary.Set(CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(1, 1) - Vector3(GRIDSIZE, 0, GRIDSIZE));
    maxBoundary.Set(CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridPos(xSize - 1, zSize - 1) + Vector3(GRIDSIZE, 0, GRIDSIZE));
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
Hero Update
********************************************************************************/
void CPlayerInfo::Update(double dt)
{
    int xSize = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMax();
    int zSize = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMax();
    if (prevIndex != index)
    {
        CGenerateHeatmap::GetInstance()->GenerateHeatmap(heatmap, xSize, zSize, index.x, index.z);
        CGenerateHeatmap::GetInstance()->CalculateDirection(heatmap, xSize, zSize);
        prevIndex = index;
    }

	if (playerMesh)
	{
		playerMesh->m_anim->animActive = true;
		playerMesh->Update(dt);
	}

	position += velocity * (float)dt;
	if (!Application::GetInstance().GetWorldBasedMousePos().IsZero())
		front.Set(Vector3(position - Application::GetInstance().GetWorldBasedMousePos()).Normalized());
	Vector3 forceDir;
	isMoving = false;
	if (!isMoving)
	{
		velocity *= 0.9f;
	}

    if (CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomCleared() &&
        CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetSpatialPartition()->GetGridType(index.x, index.z) == GRID_TYPE::DOOR)
    {
        int previousRoom = m_iCurrentRoom;

        if (CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMin() == index.x && (CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMax() >> 1) == index.z)
        {
            m_iCurrentRoom = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetDoorToRoomID(0);
            position = SpawnLocation[0];
        }
        if (CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMax() == index.x && (CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMax() >> 1) == index.z)
        {
            m_iCurrentRoom = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetDoorToRoomID(1);
            position = SpawnLocation[1];
        }
        if ((CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMax() >> 1) == index.x && CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMin() == index.z)
        {
            m_iCurrentRoom = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetDoorToRoomID(2);
            position = SpawnLocation[2];
        }
        if ((CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomXMax() >> 1) == index.x && CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetRoomZMax() == index.z)
        {
            m_iCurrentRoom = CLevel::GetInstance()->GetRoom(m_iCurrentRoom)->GetDoorToRoomID(3);
            position = SpawnLocation[3];
        }

        EntityManager::GetInstance()->GetSetSpatialPartition(m_iCurrentRoom)->Add(this);
        EntityManager::GetInstance()->GetSetSpatialPartition(previousRoom)->Remove(this);
    }

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
		if (health + healthRegen > maxHealth)
			health = maxHealth;
		else
			health += healthRegen;

		healthregenCooldownTimer = 1;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward))
	{
        forceDir.z -= 1;
		playerMesh = playerMeshes[1];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
	{
        forceDir.z += 1;
		playerMesh = playerMeshes[2];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
	{
        forceDir.x -= 1;
		playerMesh = playerMeshes[4];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
        forceDir.x += 1;
		playerMesh = playerMeshes[3];
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
			forceMagnitude = maxSpeed * DASH_DISTANCE;
			this->ApplyForce(forceDir, forceMagnitude * dt);
			isDashed = true;
			dashCooldownTimer = DASH_COOLDOWN;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		this->health -= 5;
	}
	Constrain();

	// Update the weapons
	if (KeyboardController::GetInstance()->IsKeyReleased('R'))
	{
		if (primaryWeapon)
		{
			//primaryWeapon->Reload();
			//primaryWeapon->PrintSelf();
		}
		if (secondaryWeapon)
		{
			//secondaryWeapon->Reload();
			//secondaryWeapon->PrintSelf();
		}
	}
	if (primaryWeapon)
		primaryWeapon->Update(dt);
	if (secondaryWeapon)
		secondaryWeapon->Update(dt);

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonDown(MouseController::LMB))
	{
		//if (primaryWeapon)
		//	primaryWeapon->Discharge(this->position, target, this);
	}
	if (MouseController::GetInstance()->IsButtonDown(MouseController::RMB))
	{
		//if (secondaryWeapon)
		//	secondaryWeapon->Discharge(this->position, target, this);
	}

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
}

void CPlayerInfo::Render()
{
	GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	GraphicsManager::GetInstance()->GetModelStack().Translate(
		this->position.x,
		this->position.y,
		this->position.z);
	GraphicsManager::GetInstance()->GetModelStack().Scale(
		this->scale.x,
		this->scale.y,
		this->scale.z);
	RenderHelper::RenderMesh(playerMesh);
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();

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

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
    if (position.x > maxBoundary.x - 1.0f)
    {
        position.x = maxBoundary.x - 1.0f; 
        velocity.x = 0;
    }
	//if (position.y > maxBoundary.y - 1.0f)
	//	position.y = maxBoundary.y - 1.0f;
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
    //if (position.y < minBoundary.y + 1.0f)
	//	position.y = minBoundary.y + 1.0f;
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
	cout << Math::RadianToDegree(atan2f(dir.z, dir.x)) << endl;
}

void CPlayerInfo::RecoverHealth()
{
	if (health + 10 > maxHealth)
		health = maxHealth;
	else
		health += 10;
}