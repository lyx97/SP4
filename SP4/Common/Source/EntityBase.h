#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	enum ENTITY_TYPE
	{
		DEFAULT,
        PLAYER,
		ENEMY,
		POWERUP,
		TREASURE,
		PROJECTILE,
        ENEMYPROJECTILE,
        OBSTACLE,

		NUM_ENTITYTYPE
	};

	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render(float& _renderOrder);
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
 	inline Vector3 GetPosition(){ return position; };

    inline void SetIndex(const int& _valueX, const int& _valueZ){ index.x = _valueX; index.z = _valueZ; };
    inline Vector3 GetIndex(){ return index; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	bool IsDone();
	void SetIsDone(const bool _value);

	void ApplyForce(Vector3 direction, float magnitude = 0.0f);
	float GetForce();

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool m_bCollider);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool m_bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

	// Set the entity type
	virtual void SetEntityType(ENTITY_TYPE m_eEntityType);
	// Get the entity type
	virtual ENTITY_TYPE GetEntityType(void);

	inline void SetVelocity(const Vector3& _value){ velocity = _value; };
	inline Vector3 GetVelocity(){ return velocity; };

	inline void SetMass(const float& _value){ mass = _value; };
	inline float GetMass(){ return mass; };

    inline int GetRoomID(void) const { return roomID; }
    inline void SetRoomID(const int& roomid) { roomID = roomid; }

	inline int GetHP(void) const { return health; }
	inline void SetHP(const int& health) { this->health = health; }
	inline void SetHPDamaged(const int& damage) { health -= damage; }

	inline int GetMaxHealth(void) const { return maxHealth; }
	inline void SetMaxHealth(const int& maxHealth) { this->maxHealth = maxHealth; }

    inline void SetColor(Vector3 _color) { color = _color; }
    inline Vector3 GetColor(void) { return color; }

protected:
	Vector3 position;
	Vector3 scale;
    Vector3 index;
    Vector3 maxBoundary, minBoundary;

	bool isDone;
	bool m_bCollider;
	bool m_bLaser;

	ENTITY_TYPE m_eEntityType;

	// 2D movement
	float mass;
	Vector3 velocity;
	Vector3 heatmapDir;

    int roomID;
    int health;
	int maxHealth;

    // Particle
    Vector3 color;
};

#endif // ENTITY_BASE_H