#include "PlayerInfo.h"
#include <iostream>
#include <sstream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/LaserBlaster.h"
#include "../WeaponInfo/GrenadeThrow.h"
#include "../Lua/LuaInterface.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: GenericEntity(NULL)
	, m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(10.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
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
	m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	this->scale.Set(3, 3, 3);
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	//position.Set(0, 0, 10);
    position.x = CLuaInterface::GetInstance()->getIntValue("PlayerPos_x");
    position.z = CLuaInterface::GetInstance()->getIntValue("PlayerPos_z");
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Set the pistol as the primary weapon
	primaryWeapon = new CPistol();
	primaryWeapon->Init();
    // Set the laser blaster as the secondary weapon
    secondaryWeapon = new CLaserBlaster();
    secondaryWeapon->Init();
    //secondaryWeapon = new CGrenadeThrow();
    //secondaryWeapon->Init();

    // Initialise the custom keyboard inputs
    keyMoveForward = CLuaInterface::GetInstance()->getCharValue("moveForward");
    keyMoveBackward = CLuaInterface::GetInstance()->getCharValue("moveBackward");
    keyMoveLeft = CLuaInterface::GetInstance()->getCharValue("moveLeft");
    keyMoveRight = CLuaInterface::GetInstance()->getCharValue("moveRight");

    float distanceSquare = CLuaInterface::GetInstance()->getDistanceSquareValue("CalculateDistanceSquare", Vector3(0, 0, 0), Vector3(10, 10, 10));

    int a = 1, b = 2, c = 3, d = 4;
    CLuaInterface::GetInstance()->getVariableValues("GetMinMax", a, b, c, d);

	EntityManager::GetInstance()->AddEntity(this);
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 4.0;
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

// Set m_dJumpAcceleration of the player
void CPlayerInfo::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get the terrain for the player info
GroundEntity* CPlayerInfo::GetTerrain(void)
{
    return m_pTerrain;
}

// Stop the player's movement
void CPlayerInfo::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
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

// Get m_dJumpAcceleration of the player
double CPlayerInfo::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dJumpSpeed * dt + 0.5 * m_dJumpAcceleration * dt * dt);
	target.y += (float)(m_dJumpSpeed*dt + 0.5 * m_dJumpAcceleration * dt * dt);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dJumpSpeed = m_dJumpSpeed + m_dJumpAcceleration * dt;
	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dFallSpeed * dt + 0.5 * m_dJumpAcceleration * dt * dt);
	target.y += (float)(m_dFallSpeed * dt + 0.5 * m_dJumpAcceleration * dt * dt);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * dt;
	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (position.y < m_pTerrain->GetTerrainHeight(position))
	{
		Vector3 viewDirection = target - position;
		position.y = m_pTerrain->GetTerrainHeight(position);
		target = position + viewDirection;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = false;
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	//double mouse_diff_x, mouse_diff_y;
	//MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	//double camera_yaw = mouse_diff_x * 0.0174555555555556 * 10;		// 3.142 / 180.0
	//double camera_pitch = mouse_diff_y * 0.0174555555555556 * 10;	// 3.142 / 180.0

	position += velocity * (float)dt;
	if (!Application::GetInstance().GetWorldBasedMousePos().IsZero())
		front.Set(Vector3(position - Application::GetInstance().GetWorldBasedMousePos()).Normalized());
	Vector3 forceDir;
	isMoving = false;
	if (!isMoving)
	{
		velocity *= 0.9f;
	}

	if (isDashed)
	{
		dashCooldownTimer -= dt;
		if (dashCooldownTimer <= 0)
		{
			isDashed = false;
		}
	}
	healthregenCooldownTimer -= dt;
	if (healthregenCooldownTimer <= 0)
	{
		if (health + healthRegen > maxHealth)
			health = maxHealth;
		else
			health += healthRegen;

		healthregenCooldownTimer = 1;
	}
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
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

		if (!forceDir.IsZero())
		{
			forceDir.Normalized();
			if (velocity.LengthSquared() < maxSpeed * maxSpeed)
			{
				isMoving = true;
				forceMagnitude = maxSpeed;
				this->ApplyForce(forceDir, forceMagnitude * dt);
			}
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
	}

	//if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward) ||
	//	KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward) ||
	//	KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft) ||
	//	KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	//{
	//	Vector3 viewVector = target - position;
	//	Vector3 rightUV;
 //       if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward))
	//	{
	//		Vector3 temp(viewVector);
	//		temp.y = 0;
	//		position += temp.Normalized() * (float)m_dSpeed * (float)dt;
	//	}
 //       else if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
	//	{
	//		Vector3 temp(viewVector);
	//		temp.y = 0;
	//		position -= temp.Normalized() * (float)m_dSpeed * (float)dt;
	//	}
 //       if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
	//	{
	//		rightUV = (viewVector.Normalized()).Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		position -= rightUV * (float)m_dSpeed * (float)dt;
	//	}
 //       else if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	//	{
	//		rightUV = (viewVector.Normalized()).Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		position += rightUV * (float)m_dSpeed * (float)dt;
	//	}
	//	// Constrain the position
	//	Constrain();
	//	// Update the target
	//	target = position + viewVector;
	//}

	// Rotate the view direction
	/*if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_UP) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	{
		Vector3 viewUV = (target - position).Normalized();
		Vector3 rightUV;
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		{
			float yaw = (float)m_dSpeed * (float)dt;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		{
			float yaw = (float)(-m_dSpeed * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			float pitch = (float)(m_dSpeed * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			float pitch = (float)(-m_dSpeed * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
	}*/

	//Update the camera direction based on mouse move
	//{
	//	Vector3 viewUV = (target - position).Normalized();
	//	Vector3 rightUV;

	//	{
	//		float yaw = (float)(-m_dSpeed * camera_yaw * (float)dt);
	//		Mtx44 rotation;
	//		rotation.SetToRotation(yaw, 0, 1, 0);
	//		viewUV = rotation * viewUV;
	//		target = position + viewUV;
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//	}
	//	{
	//		float pitch = (float)(-m_dSpeed * camera_pitch * (float)dt);
	//		rightUV = viewUV.Cross(up);
	//		rightUV.y = 0;
	//		rightUV.Normalize();
	//		up = rightUV.Cross(viewUV).Normalized();
	//		Mtx44 rotation;
	//		rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
	//		viewUV = rotation * viewUV;
 //           if (viewUV.y <= 0.9f && viewUV.y >= -0.6f)
 //           {
 //               target = position + viewUV;
 //           }
	//	}
	//}

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
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("sphere"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	//if (position.y > maxBoundary.y - 1.0f)
	//	position.y = maxBoundary.y - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	//if (position.y < minBoundary.y + 1.0f)
	//	position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the player is not jumping nor falling, then adjust his y position
	if ((m_bJumpUpwards == false) && (m_bFallDownwards == false))
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		if (position.y != m_pTerrain->GetTerrainHeight(position))
			position.y = m_pTerrain->GetTerrainHeight(position);
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