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

	playerMeshes[0] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_down");					// idle
	playerMeshes[1] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_up");					// up
	playerMeshes[2] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_down");					// down
	playerMeshes[3] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_left");					// left
	playerMeshes[4] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_right");				// right
	playerMeshes[5] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingup");			// shoot up
	playerMeshes[6] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingupright");		// shoot up right
	playerMeshes[7] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingright");		// shoot right
	playerMeshes[8] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingdownright");	// shoot down right
	playerMeshes[9] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingdown");			// shoot down
	playerMeshes[10] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingdownleft");	// shoot down left
	playerMeshes[11] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingleft");		// shoot left
	playerMeshes[12] = (SpriteAnimation*)MeshBuilder::GetInstance()->GetMesh("player_shootingupleft");		// shoot up left

	for (int i = 0; i < 5; ++i)
	{
		playerMeshes[i]->m_anim = new Animation;
		playerMeshes[i]->m_anim->Set(0, 13, 1, 1.f, true);
	}
	for (int i = 5; i < 13; ++i)
	{
		playerMeshes[i]->m_anim = new Animation;
		playerMeshes[i]->m_anim->Set(0, 3, 1, 1.f, true);
	}
	playerMesh = playerMeshes[0];

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
		//playerMesh = playerMeshes[1];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
	{
        forceDir.z += 1;
		//playerMesh = playerMeshes[2];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
	{
        forceDir.x -= 1;
		//playerMesh = playerMeshes[4];
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
        forceDir.x += 1;
		//playerMesh = playerMeshes[3];
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
	 // DEBUGGING TOOLS
	if (KeyboardController::GetInstance()->IsKeyPressed('O'))
	{
		this->health -= 10;
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
}

void CPlayerInfo::Render()
{
	//GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	//GraphicsManager::GetInstance()->GetModelStack().Translate(
	//	this->position.x,
	//	this->position.y,
	//	this->position.z);
	//GraphicsManager::GetInstance()->GetModelStack().Scale(
	//	this->scale.x,
	//	this->scale.y,
	//	this->scale.z);
	//RenderHelper::RenderMesh(playerMesh);
	//GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
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

	//if (-120 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < -60)
	//	playerMesh = playerMeshes[5];
	//else if (-60 < Math::RadianToDegree(atan2f(dir.z, dir.x)) && 
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < -30)
	//	playerMesh = playerMeshes[6];
	//else if (-30 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < 30)
	//	playerMesh = playerMeshes[7];
	//else if (30 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < 60)
	//	playerMesh = playerMeshes[8];
	//else if (60 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < 120)
	//	playerMesh = playerMeshes[9];
	//else if (120 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < 165)
	//	playerMesh = playerMeshes[10];
	//else if (165 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < -165)
	//	playerMesh = playerMeshes[11];
	//else if (-165 < Math::RadianToDegree(atan2f(dir.z, dir.x)) &&
	//	Math::RadianToDegree(atan2f(dir.z, dir.x)) < -120)
	//	playerMesh = playerMeshes[12];
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
