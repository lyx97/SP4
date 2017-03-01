#include "Ghast.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL/glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Level/Level.h"

#include <iostream>
using namespace std;

CGhast::CGhast(void)
    : CProjectile(NULL)
    , prevIndex(Vector3(0, 0, 0))
{
}

CGhast::CGhast(Mesh* _modelMesh)
    : CProjectile(_modelMesh)
    , prevIndex(Vector3(0, 0, 0))
{
}

CGhast::~CGhast(void)
{
    modelMesh = NULL;
    theSource = NULL;
}

// Update the status of this projectile
void CGhast::Update(double dt)
{
    if (m_bStatus == false)
        return;

    if (prevIndex != index)
    {
        if (CLevel::GetInstance()->
            GetRoom(CPlayerInfo::GetInstance()
            ->GetRoomID())
            ->GetSpatialPartition()
            ->GetGridType(index.x, index.z)
            == GRID_TYPE::WALL)
        {
            SetStatus(false);
            SetIsDone(true);
            return;
        }
        prevIndex = index;
    }

    // Update TimeLife of projectile. Set to inactive if too long
    m_fLifetime -= (float)dt;
    if (m_fLifetime < 0.0f)
    {
        SetStatus(false);
        SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
        return;
    }

    theDirection = (CPlayerInfo::GetInstance()->GetPosition() - position).Normalized();

    // Update Position
    position += theDirection * dt * m_fSpeed;
}

// Render this projectile
void CGhast::Render(float& _renderOrder)
{
    if (m_bStatus == false)
        return;

    if (m_fLifetime < 0.0f)
        return;

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(15, 15, 15);
    RenderHelper::RenderMesh(modelMesh);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

// Create a projectile and add it into EntityManager
CGhast* Create::Ghast(const std::string& _meshName,
    const Vector3& _position,
    const Vector3& _direction,
    const float m_fLifetime,
    const float m_fSpeed,
	const float damage)
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    CGhast* result = new CGhast(modelMesh);
    result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetDamage(damage);
    result->SetMass(50);
    result->SetStatus(true);
    result->SetCollider(true);
    result->SetEntityType(EntityBase::ENEMYPROJECTILE);
    result->SetVelocity(_direction); // to get the velocity outside the class
    result->SetRoomID(CPlayerInfo::GetInstance()->GetRoomID());
    result->SetCollider(true);
    int x = 15; int y = 15;
    x = (x >> 1) - 2; y = (y >> 1) - 2;
    result->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));


    EntityManager::GetInstance()->AddEntity(result, CPlayerInfo::GetInstance()->GetRoomID());
    return result;
}