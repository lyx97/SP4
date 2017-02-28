#pragma once
#include "../Projectile/Projectile.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"

class Mesh;
class CPlayerInfo;
class CLevel;

class CGhast : public CProjectile
{
public:
    CGhast(void);
    CGhast(Mesh* _modelMesh);
    ~CGhast(void);
public:

    // Update the status of this projectile
    void Update(double dt = 0.0333f);
    // Render this projectile
    void Render(void);
private:
    Vector3 prevIndex;
};

namespace Create
{
    CGhast* Ghast(const std::string& _meshName,
        const Vector3& _position,
        const Vector3& _direction,
        const float m_fLifetime,
        const float m_fSpeed);
};

