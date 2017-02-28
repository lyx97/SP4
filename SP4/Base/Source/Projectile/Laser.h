#pragma once

#include "EntityBase.h"
#include "Vector3.h"
#include "../GenericEntity.h"

class Mesh;
class CPlayerInfo;
class CLevel;

class CLaser : public GenericEntity
{
public:
	CLaser(void);
	CLaser(Mesh* _modelMesh);
	~CLaser(void);
public:
	void SetLength(const float m_fLength);
	float GetLength(void) const;
	void CalculateAngles(void);

	// Update the status of this projectile
    virtual void Update(double dt = 0.0333f);
	// Render this projectile
    virtual void Render(float& _renderOrder);

	// Set the position and direction of the projectile
	void Set(Vector3 theNewPosition,
		Vector3 theNewDirection,
		const float m_fLifetime,
		const float m_fSpeed);

	inline bool GetStatus(void) const { return this->m_bStatus; };
	inline void SetStatus(const bool m_bStatus){ this->m_bStatus = m_bStatus; };

	inline unsigned int GetDamage(){ return this->damage; };
	inline void SetDamage(unsigned int damage){ this->damage = damage; };

private:
    Vector3 prevIndex;

	float m_fLength;
	float angle_x, angle_y, angle_z;

	bool m_bStatus;
	float m_fLifetime;
	float m_fSpeed;
	Vector3 theDirection;
	unsigned int damage;
};

namespace Create
{
	CLaser* Laser(const std::string& _meshName, 
					const Vector3& _position, 
					const Vector3& _direction, 
					const float m_fLength, 
					const float m_fLifetime, 
					const float m_fSpeed,
					const float m_fDamage);
};

