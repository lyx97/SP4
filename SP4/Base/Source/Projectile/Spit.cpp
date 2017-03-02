#include "Spit.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL/glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Level/Level.h"
#include "LoadTGA.h"
#include "../BatchRendering.h"

#include <iostream>
using namespace std;

CSpit::CSpit(void)
    : CProjectile(NULL)
    , prevIndex(Vector3(0, 0, 0))
{
    spit = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 6);
    spit->textureID = LoadTGA("Image//Projectile//spit.tga");
    spit->m_anim = new Animation;
    spit->m_anim->Set(0, 5, 1, 5.f, true);

    // Particle
    color.Set(Vector3(0.f, 1.f, 0.f));
}

CSpit::~CSpit(void)
{
    modelMesh = NULL;
    theSource = NULL;
    delete spit;
}

// Update the status of this projectile
void CSpit::Update(double dt)
{
    if (m_bStatus == false)
        return;

    bool done = false;

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
            done = true;
        }
        prevIndex = index;
    }

    if (theDirection.y > 0)
        theDirection.y -= dt * m_fSpeed;
    else if (position.y > 0)
        theDirection.y -= dt * m_fSpeed;


    // Update Position
    position += theDirection.Normalized() * dt * m_fSpeed;

    if (position.y <= 0)
    {
        SetStatus(false);
        SetIsDone(true);
        done = true;
    }

    if (spit)
    {
        spit->m_anim->animActive = true;
        spit->Update(dt * 1.f);
    }

    if (spit->GetCurrentFrame() == spit->m_anim->endFrame)
    {
        SetStatus(false);
        SetIsDone(true);
        done = true;
    }

    if (done)
    {
        BatchRendering::GetInstance()->GetParticle(
            position,
            Vector3(2.f, 2.f, 2.f),
            color,
            Vector3(0.f, 0.f, 0.f),
            NUMPARTICLESPAWN);
    }
}

// Render this projectile
void CSpit::Render(float& _renderOrder)
{
    if (m_bStatus == false)
        return;

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(30, 30, 30);
    RenderHelper::RenderMesh(spit);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

// Create a projectile and add it into EntityManager
CSpit* Create::Spit(
    const Vector3& _position,
    const Vector3& _direction,
    const float m_fSpeed,
	const float damage)
{
    CSpit* result = new CSpit();
    result->Set(_position, _direction, 0, m_fSpeed);
    result->AddUpwardForce(_direction.Length() * 0.5f);
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