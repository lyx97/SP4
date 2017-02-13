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
        NUM_ENTITYTYPE
    };

	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	bool IsDone();
	void SetIsDone(const bool _value);

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

protected:
	Vector3 position;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;
    bool m_bLaser;

    ENTITY_TYPE m_eEntityType;
};

#endif // ENTITY_BASE_H