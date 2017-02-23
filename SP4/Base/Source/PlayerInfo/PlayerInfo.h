#pragma once

#include "Vector3.h"
#include "../Camera/Camera.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "../GenericEntity.h"
#include "../TextEntity.h"
#include "../Level/GenerateHeatmap.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../Items/Treasure.h"

class Treasure;

class CPlayerInfo : public GenericEntity
{
protected:
	static CPlayerInfo *s_instance;
	CPlayerInfo(void);

    int m_iCurrentRoom;

    Vector3 prevIndex;
    Vector3 SpawnLocation[4];

    CHeatmap** heatmap;

public:
	static CPlayerInfo *GetInstance()
	{
		if (!s_instance)
			s_instance = new CPlayerInfo;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}
	~CPlayerInfo(void);

	// Initialise this class instance
	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);	
    // Set boundary
	void SetBoundary(Vector3 max, Vector3 min);

	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;

    // Room
    inline const int GetRoomID(void) { return m_iCurrentRoom; }
    inline void SetRoomID(const int& roomID) { m_iCurrentRoom = roomID; }

	// Update
	void Update(double dt = 0.0333f);
	void Render();

	// Constrain the position within the borders
	void Constrain(void);

	// Handling Camera
	void AttachCamera(Camera* _cameraPtr);
	void DetachCamera();
	void RecoverHealth();
	void Shoot(Vector3 dir);
	inline float GetHealth(){ return health; };
	inline void SetHealth(float health){ this->health = health; };

	inline float GetMaxHealth(){ return maxHealth; };
	inline void SetMaxHealth(float maxHealth){ this->maxHealth = maxHealth; };

	inline float GetMaxSpeed(){ return maxSpeed; };
	inline void SetMaxSpeed(float maxSpeed){ this->maxSpeed = maxSpeed; }

	inline float GetHealthRegen(){ return healthregen; };
	inline void SetHealthRegen(float healthregen){ this->healthregen = healthregen; };

	void AddTreasures(int type);
	void UpdateTreasures(double dt);

    inline CHeatmap** GetHeatmap() { return heatmap; }

	inline Treasure* GetTreasure() { return this->treasure; };
	int killCount;
	
private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;

	double m_dSpeed;
	double m_dAcceleration;

	bool m_bJumpUpwards;
	double m_dJumpSpeed;
	double m_dJumpAcceleration;

	double m_dFallSpeed;
	bool m_bFallDownwards;
	double m_dFallAcceleration;

	Camera* attachedCamera;

	CWeaponInfo* primaryWeapon;
	CWeaponInfo* secondaryWeapon;

	SpriteAnimation* spriteAnimation;
	SpriteAnimation* playerMeshes[13];
	SpriteAnimation* playerMesh;

	// Key to move the player
	char keyMoveForward;
	char keyMoveBackward;
	char keyMoveLeft;
	char keyMoveRight;

	// 2D movements
	float forceMagnitude = 0;
	bool isMoving;
	bool isDashed;

	// player stats
	float maxHealth;
	float health;
	float maxSpeed;
	float dashDistance;
	float dashCooldown;
	float dashCooldownTimer;
	float healthregen;
	float healthregenCooldown;
	float healthregenCooldownTimer;

	Treasure* treasure;
};