#include "Scythe.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL/glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Level/Level.h"
#include "LoadTGA.h"

#include <iostream>
using namespace std;

CScythe::CScythe(void)
    : CProjectile(NULL)
    , prevIndex(Vector3(0, 0, 0))
    , m_dChangeDir(0.0)
    , rotation(0)
    , m_bRoamMode(false)
{
    scythe = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 2);
    scythe->textureID = LoadTGA("Image//Boss//reaper_scythe.tga");
    scythe->m_anim = new Animation;
    scythe->m_anim->Set(0, 1, 1, 1.f, true);
}

CScythe::~CScythe(void)
{
    modelMesh = NULL;
    theSource = NULL;
    delete scythe;
}

// Update the status of this projectile
void CScythe::Update(double dt)
{
    if (m_bStatus == false)
        return;

    if (!m_bRoamMode)
    {
        m_fSpeed -= (float)dt * 0.1f;
        if (m_fSpeed <= 0)
        {
            SetStatus(false);
            SetIsDone(true);
            return;
        }

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
    }
    else
    {
        // Update TimeLife of projectile. Set to inactive if too long
        m_fLifetime -= (float)dt;
        if (m_fLifetime < 0.0f)
        {
            m_bStatus = false;
            SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
            return;
        }

        m_fSpeed = 1.f;
        m_dChangeDir += dt;
        if (m_dChangeDir >= 1.0)
        {
            theDirection = CPlayerInfo::GetInstance()->GetPosition() - position;
            m_dChangeDir = 0.0;
        }
    }

    // Update Position
    position += theDirection * dt * m_fSpeed;

    if (scythe)
    {
        scythe->m_anim->animActive = true;
        scythe->Update(dt * 1.f);
    }

    rotation += dt * 1000;
    if (rotation >= 10000)
        rotation = 0;
}

// Render this projectile
void CScythe::Render(float& _renderOrder)
{
    if (m_bStatus == false)
        return;

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(rotation, 0, 1, 0);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, 1);
    RenderHelper::RenderMesh(scythe);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

// Create a projectile and add it into EntityManager
CScythe* Create::Scythe(
    const Vector3& _position,
    const Vector3& _direction,
    const float m_fSpeed)
{
    CScythe* result = new CScythe();
    result->Set(_position, _direction, 0, m_fSpeed);
    result->SetScale(Vector3(30, 30, 0));
    result->SetMass(50);
    result->SetStatus(true);
    result->SetCollider(true);
    result->SetEntityType(EntityBase::ENEMYPROJECTILE);
    result->SetVelocity(_direction); // to get the velocity outside the class
    result->SetRoomID(CPlayerInfo::GetInstance()->GetRoomID());
    result->SetCollider(true);
    int x = result->GetScale().x; int y = result->GetScale().y;
    x = (x >> 1) - 4; y = (y >> 1) - 4;
    result->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(result, CPlayerInfo::GetInstance()->GetRoomID());
    return result;
}

CScythe* Create::Scythe(
    const Vector3& _position,
    const Vector3& _direction,
    const float m_fLifetime,
    const float m_fSpeed,
    const bool _mode)
{
    CScythe* result = new CScythe();
    result->Set(_position, _direction, m_fLifetime, m_fSpeed);
    result->SetScale(Vector3(60, 60, 0));
    result->SetMass(50);
    result->SetStatus(true);
    result->SetCollider(true);
    result->SetEntityType(EntityBase::ENEMYPROJECTILE);
    result->SetVelocity(_direction); // to get the velocity outside the class
    result->SetRoomID(CPlayerInfo::GetInstance()->GetRoomID());
    result->SetCollider(true);
    result->SetMode(_mode);
    int x = result->GetScale().x; int y = result->GetScale().y;
    x = (x >> 1) - 8; y = (y >> 1) - 8;
    result->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(result, CPlayerInfo::GetInstance()->GetRoomID());
    return result;
}