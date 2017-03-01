#pragma once

#include <list>
#include <iostream>
#include <sstream>

#include "Vector3.h"
#include "../GenericEntity.h"
#include "../TextEntity.h"
#include "../Level/GenerateHeatmap.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../Items/Treasure.h"

class Treasure;

enum ANIMATION
{
    WALK,
    SHOOT,

    NUM_ANIMATION
};

static const float MAX_DREAMBAR = 100.f;

class CPlayerInfo : public GenericEntity
{
protected:
	static CPlayerInfo *s_instance;
	CPlayerInfo(void);

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

    // Set boundary
	void SetBoundary(Vector3 max, Vector3 min);

	// Update
	void Update(double dt = 0.0333f);
    void Render(float& _renderOrder);
	void RenderUI(void);

	// Constrain the position within the borders
	void Constrain(void);

    inline Vector3 GetDirection(void) const { return direction; }
    inline void SetDirection(Vector3 dir) { direction = dir; }
    
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
	void Revert();

    inline CHeatmap** GetHeatmap() { return heatmap; }

	inline Treasure* GetTreasure() { return this->treasure; };
	inline float GetTreasuseDuration() { return this->treasureDurationTimer; };

	inline float GetDreamBar() { return this->dreamBar; };
	inline void SetDreamBar(float dreamBar) { this->dreamBar = dreamBar; };
	inline float GetDreamBarRatio() { return this->dreamBar / MAX_DREAMBAR; };

	inline unsigned int GetDamage() { return this->damage; };
	inline void SetDamage(unsigned int damage) { this->damage = damage; };

	inline double GetTimeBetweenShots() { return this->timeBetweenShots; };
	inline void SetTimeBetweenShots(double timeBetweenShots) { this->timeBetweenShots = timeBetweenShots; };

	inline bool IsInvincible() { return this->invincible; };
	inline void SetIsInvincible(bool invincible) { this->invincible = invincible; };

	inline bool IsOnScreenUI() { return this->onScreenUI; };
	inline void SetIsOnScreenUI(bool onScreenUI) { this->onScreenUI = onScreenUI; };
	int killCount;
	
private:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
    Vector3 direction;
    Vector3 prevPos;

	double m_dSpeed;
	double m_dAcceleration;

	SpriteAnimation* playerMeshes[2];

    SpriteAnimation* currentAnimation;

	// Key to move the player
	char keyMoveForward;
	char keyMoveBackward;
	char keyMoveLeft;
	char keyMoveRight;

	// 2D movements
	float forceMagnitude = 0;
	bool isMoving;
	bool isDashed;
	bool usingTreasure;

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
	float treasureDurationTimer;
	float dreamBar;
	unsigned int damage;
	float invincibleTimer;
	bool invincible;

	// Treasure
	Treasure* treasure;
	float defaultHealthRegenCooldown;
	float defaultSpeed;
	
    // Room
    Vector3 prevIndex;
    Vector3 SpawnLocation[4];

    CHeatmap** heatmap;

    // Animation
    Vector3 rotateLeftLeg;
    Vector3 rotateRightLeg;

    bool rotateLLUP;

    std::list<ANIMATION> animationList;

	TextEntity* textOBJ[5];
	float fontSize = 25.0f;

	// UI
	bool onScreenUI;
	int UIScale;
	float prevHealth;
	float healthRatio;
	float healthScale;
	float dashScale;
	float dreambarScale;
	float treasureScale;
	std::ostringstream stats;

	// projectile
	double elapsedTime;
	double timeBetweenShots;
	bool fire;
};