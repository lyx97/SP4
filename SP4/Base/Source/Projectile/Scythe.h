#pragma once

#include "../Projectile/Projectile.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"

class Mesh;
class CPlayerInfo;
class CLevel;

class CScythe : public CProjectile
{
public:
    CScythe(void);
    ~CScythe(void);
public:
    // Update the status of this projectile
    virtual void Update(double dt = 0.0333f);
    // Render this projectile
    virtual void Render(float& _renderOrder);

    inline void SetMode(bool _mode) { m_bRoamMode = _mode; }

private:
    Vector3 prevIndex;

    SpriteAnimation* scythe;

    double m_dChangeDir;

    float rotation;

    bool m_bRoamMode;
};

namespace Create
{
    CScythe* Scythe(
        const Vector3& _position,
        const Vector3& _direction,
        const float m_fSpeed);

    CScythe* Scythe(
        const Vector3& _position,
        const Vector3& _direction,
        const float m_fLifetime,
        const float m_fSpeed,
        const bool _mode);
};