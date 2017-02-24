#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	enum ENTITY_TYPE
	{
		DEFAULT,
		ENEMY,
		ITEM,
		PROJECTILE,

		NUM_ENTITYTYPE
	};

	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
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

	inline void SetFront(const Vector3& _value){ front = _value; };
	inline Vector3 GetFront(){ return front; };

	inline void SetMass(const float& _value){ mass = _value; };
	inline float GetMass(){ return mass; };

    inline int GetRoomID(void) const { return roomID; }
    inline void SetRoomID(const int& roomid) { roomID = roomid; }

protected:
	Vector3 position;
	Vector3 scale;
    Vector3 index;

	bool isDone;
	bool m_bCollider;
	bool m_bLaser;

	ENTITY_TYPE m_eEntityType;

	// 2D movement
	float mass;
	Vector3 velocity;
	Vector3 front;

    int roomID;
};

#endif // ENTITY_BASE_H