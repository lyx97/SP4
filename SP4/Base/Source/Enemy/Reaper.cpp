#include "Reaper.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "../Level/Level.h"
#include "../Projectile/Spit.h"
#include "LoadTGA.h"

Reaper::Reaper(const int _roomID)
    : targetPos(Vector3(0, 0, 0))
    , m_dSpeed(30.0)
    , m_dResponseTime(0.0)
    , m_bAttackAnimation(false)
    , m_bAlive(true)
{
    this->position = Vector3(100, 0, 0);
    this->scale = Vector3(100, 100, 0);
    this->velocity = Vector3(0, 0, 0);

    this->isDone = false;
    this->m_bCollider = true;
    this->m_bLaser = false;

    this->m_eEntityType = EntityBase::ENEMY;

    moveLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    moveLeft->textureID = LoadTGA("Image//Boss//reaper_moveleft.tga");
    moveLeft->m_anim = new Animation;
    moveLeft->m_anim->Set(0, 3, 1, 1.f, true);

    moveRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    moveRight->textureID = LoadTGA("Image//Boss//reaper_moveright.tga");
    moveRight->m_anim = new Animation;
    moveRight->m_anim->Set(0, 3, 1, 1.f, true);

    attackLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    attackLeft->textureID = LoadTGA("Image//Boss//reaper_attackleft.tga");
    attackLeft->m_anim = new Animation;
    attackLeft->m_anim->Set(0, 4, 1, 2.0f, true);
    attackLeft->SetAttackFrame(11);

    attackRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    attackRight->textureID = LoadTGA("Image//Boss//reaper_attackright.tga");
    attackRight->m_anim = new Animation;
    attackRight->m_anim->Set(0, 4, 1, 2.0f, true);
    attackRight->SetAttackFrame(11);

    dieLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 6);
    dieLeft->textureID = LoadTGA("Image//Boss//reaper_dieleft.tga");
    dieLeft->m_anim = new Animation;
    dieLeft->m_anim->Set(0, 5, 1, 1.f, true);

    dieRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 6);
    dieRight->textureID = LoadTGA("Image//Boss//reaper_dieright.tga");
    dieRight->m_anim = new Animation;
    dieRight->m_anim->Set(0, 5, 1, 1.f, true);

    smoke = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    smoke->textureID = LoadTGA("Image//Boss//reaper_smoke.tga");
    smoke->m_anim = new Animation;
    smoke->m_anim->Set(0, 4, 1, 1.f, true);

    scythe = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 2);
    scythe->textureID = LoadTGA("Image//Boss//reaper_scythe.tga");
    scythe->m_anim = new Animation;
    scythe->m_anim->Set(0, 1, 1, 1.f, true);

    currentAnimation = moveLeft;

	health = 100;
    roomID = _roomID;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 30; y = (y >> 1) - 30;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Boundary
    CRoom* room = CLevel::GetInstance()->GetRoom(roomID);
    int xSize = room->GetRoomXMax();
    int zSize = room->GetRoomZMax();
    minBoundary.Set(room->GetSpatialPartition()->GetGridPos(1, 1) - Vector3(GRIDSIZE, 0, GRIDSIZE));
    maxBoundary.Set(room->GetSpatialPartition()->GetGridPos(xSize - 1, zSize - 1) + Vector3(GRIDSIZE, 0, GRIDSIZE));
}

Reaper::~Reaper()
{
    delete moveLeft;
    delete moveRight;
    delete attackLeft;
    delete attackRight;
    delete dieLeft;
    delete dieRight;
    delete smoke;
}

void Reaper::Update(double dt)
{
    CHeatmap** heatmap = CPlayerInfo::GetInstance()->GetHeatmap();

    int x = index.x;
    int z = index.z;

    velocity = heatmap[x][z].GetDir();

    m_dResponseTime += dt;

	if (health <= 0)
    {
        fsm = FSM::DEAD;
    }
    else
    {
        if ((position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() <= 2000
            && m_dResponseTime >= m_dResponse)
        {
            fsm = FSM::ATTACK;
            m_dResponseTime = 0.0;
        }
        else if (m_dResponseTime >= m_dResponse)
        {
            fsm = FSM::MOVE;
            m_dResponseTime = 0.0;
        }
    }

    switch (fsm)
    {
    case MOVE:
        //position += velocity.Normalized() * dt * m_dSpeed;

        if (velocity.x > 0)
            currentAnimation = moveRight;
        else
            currentAnimation = moveLeft;
        break;
    case ATTACK:
        if (velocity.x > 0)
            currentAnimation = attackRight;
        else
            currentAnimation = attackLeft;

        if (currentAnimation->GetAttack() && m_bAttackAnimation)
        {
            Vector3 temp = CPlayerInfo::GetInstance()->GetPosition() - position;
            CSpit* spit = Create::Spit(
                position,
                temp,
                60.f,
				damage);

            m_bAttackAnimation = false;
        }
        if (!currentAnimation->GetAttack())
            m_bAttackAnimation = true;

        break;
    case DEAD:
        if (velocity.x > 0)
            currentAnimation = dieRight;
        else
            currentAnimation = dieLeft;
        break;
    }

    if (currentAnimation)
    {
        currentAnimation->m_anim->animActive = true;
        currentAnimation->Update(dt * 1.f);

        if (fsm == FSM::DEAD && currentAnimation->GetCurrentFrame() == currentAnimation->m_anim->endFrame)
            SetIsDone(true);
    }

    if (scythe)
    {
        scythe->m_anim->animActive = true;
        scythe->Update(dt * 1.f);
    }

    if (prevHP != health)
    {
        prevHP = health;
        HPScale = ((float)health / 100.f) * 100.f;
    }

    bob += dt * 1000;

    Enemy2D::Update(dt);
}

void Reaper::Render(float& _renderOrder)
{
	Enemy2D::Render(_renderOrder);

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z + 1);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    //RenderHelper::RenderMesh(smoke);
    RenderHelper::RenderMesh(currentAnimation);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z + 1);
    modelStack.Rotate(bob, 0, 1, 0);
    modelStack.Rotate(90, -1, 0, 0);    
    modelStack.Scale(scale.x * 2, scale.y * 2, scale.z * 2);
    //RenderHelper::RenderMesh(smoke);
    RenderHelper::RenderMesh(scythe);
    modelStack.PopMatrix();

    // FOR HP BAR
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + (scale.y * 0.5f), position.z);
    modelStack.Scale(HPScale, 20, 1);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("wall"));
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void Reaper::Constrain()
{
    // Constrain player within the boundary
    if (position.x > maxBoundary.x - 1.0f)
    {
        position.x = maxBoundary.x - 1.0f;
        velocity.x = 0;
    }
    if (position.z > maxBoundary.z - 1.0f)
    {
        position.z = maxBoundary.z - 1.0f;
        velocity.z = 0;
    }
    if (position.x < minBoundary.x + 1.0f)
    {
        position.x = minBoundary.x + 1.0f;
        velocity.x = 0;
    }
    if (position.z < minBoundary.z + 1.0f)
    {
        position.z = minBoundary.z + 1.0f;
        velocity.z = 0;
    }
}