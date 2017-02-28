#pragma once

#include "../Projectile/Projectile.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"

class Mesh;
class CPlayerInfo;
class CLevel;

class CSpit : public CProjectile
{
public:
    CSpit(void);
    ~CSpit(void);
public:

    // Update the status of this projectile
    virtual void Update(double dt = 0.0333f);
    // Render this projectile
    virtual void Render(float& _renderOrder);

    inline void AddUpwardForce(const float& _value) { theDirection.y += _value; }
private:
    Vector3 prevIndex;

    SpriteAnimation* spit;
};

namespace Create
{
    CSpit* Spit(
        const Vector3& _position,
        const Vector3& _direction,
        const float _upwardForce,
        const float m_fSpeed);
};

