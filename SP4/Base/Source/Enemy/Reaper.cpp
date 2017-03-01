#include "Reaper.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "../Level/Level.h"
#include "../Projectile/Scythe.h"
#include "LoadTGA.h"

Reaper::Reaper(const int _roomID)
    : targetPos(Vector3(0, 0, 0))
    , m_dSpeed(30.0)
    , m_dResponseTime(0.0)
    , m_bAttackAnimation(false)
    , m_bAlive(true)
    , m_bInvulnerable(true)
{
    this->position = Vector3(0, 0, 0);
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

    moveLeft2 = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
    moveLeft2->textureID = LoadTGA("Image//Boss//reaper2_moveleft.tga");
    moveLeft2->m_anim = new Animation;
    moveLeft2->m_anim->Set(0, 6, 1, 1.f, true);

    moveRight2 = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
    moveRight2->textureID = LoadTGA("Image//Boss//reaper2_moveright.tga");
    moveRight2->m_anim = new Animation;
    moveRight2->m_anim->Set(0, 6, 1, 1.f, true);

    attackLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    attackLeft->textureID = LoadTGA("Image//Boss//reaper_attackleft.tga");
    attackLeft->m_anim = new Animation;
    attackLeft->m_anim->Set(0, 4, 1, 2.0f, true);
    attackLeft->SetAttackFrame(4);

    attackRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    attackRight->textureID = LoadTGA("Image//Boss//reaper_attackright.tga");
    attackRight->m_anim = new Animation;
    attackRight->m_anim->Set(0, 4, 1, 2.0f, true);
    attackRight->SetAttackFrame(4);

    scytheattackLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    scytheattackLeft->textureID = LoadTGA("Image//Boss//reaper_scytheattackleft.tga");
    scytheattackLeft->m_anim = new Animation;
    scytheattackLeft->m_anim->Set(0, 4, 1, 2.0f, true);
    scytheattackLeft->SetAttackFrame(4);

    scytheattackRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    scytheattackRight->textureID = LoadTGA("Image//Boss//reaper_attackright.tga");
    scytheattackRight->m_anim = new Animation;
    scytheattackRight->m_anim->Set(0, 4, 1, 2.0f, true);
    scytheattackRight->SetAttackFrame(4);

    dieLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 6);
    dieLeft->textureID = LoadTGA("Image//Boss//reaper_dieleft.tga");
    dieLeft->m_anim = new Animation;
    dieLeft->m_anim->Set(0, 5, 1, 1.f, true);

    dieRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 6);
    dieRight->textureID = LoadTGA("Image//Boss//reaper_dieright.tga");
    dieRight->m_anim = new Animation;
    dieRight->m_anim->Set(0, 5, 1, 1.f, true);

    currentAnimation = moveLeft;

    maxHealth = 100;
    health = maxHealth;
    roomID = _roomID;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 30; y = (y >> 1) - 30;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Boundary
    minBoundary.Set(CPlayerInfo::GetInstance()->GetMinBoundary());
    maxBoundary.Set(CPlayerInfo::GetInstance()->GetMaxBoundary());

    // Smoke
    Vector3 smokePos[4];
    smokePos[0] = Vector3(position.x - 125, 0, position.z);
    smokePos[1] = Vector3(position.x + 125, 0, position.z);
    smokePos[2] = Vector3(position.x, 0, position.z - 125);
    smokePos[3] = Vector3(position.x, 0, position.z + 125);

    for (int i = 0; i < 4; ++i)
    {
        m_bSmokeActive[i] = true;
        smoke[i] = new Smoke(smokePos[i], roomID);
    }
}

Reaper::~Reaper()
{
    delete moveLeft;
    delete moveRight;
    delete attackLeft;
    delete attackRight;
    delete scytheattackLeft;
    delete scytheattackRight;
    delete dieLeft;
    delete dieRight;

    delete moveLeft2;
    delete moveRight2;
    //delete attackLeft2;
    //delete attackRight2;
    //delete dieLeft2;
    //delete dieRight2;
}

void Reaper::Update(double dt)
{
    CHeatmap** heatmap = CPlayerInfo::GetInstance()->GetHeatmap();

    int x = index.x;
    int z = index.z;

    velocity = heatmap[x][z].GetDir();

    m_dResponseTime += dt;

    if (m_bInvulnerable)
    {
        health = maxHealth;

        for (int i = 0; i < 4; ++i)
        {
            if (!smoke[i]->IsDone())
                m_bSmokeActive[i] = true;
            else
                m_bSmokeActive[i] = false;
        }

        m_bInvulnerable = false;
        for (int i = 0; i < 4; ++i)
        {
            if (m_bSmokeActive[i])
            {
                m_bInvulnerable = true;
                break;
            }
        }
    }

    Vector3 temp = CPlayerInfo::GetInstance()->GetPosition() - position;

	if (health <= 0)
    {
        fsm = FSM::DEAD;
    }
    else
    {
        if (m_bInvulnerable)
        {
            fsm = FSM::SMOKEATTACK;
        }
        else
        {
            //if (m_dResponseTime >= m_dResponse)
            //{
            //    if (temp.LengthSquared() <= 3000)
            //        fsm = FSM::SCYTHEATTACK;
            //    else
            //        fsm = FSM::MOVE;

            //    m_dResponseTime = 0.0;
            //}
            fsm = FSM::SCYTHEATTACK;

        }
    }

    switch (fsm)
    {
    //case MOVE:
    //    position += velocity * dt * m_dSpeed * 2;

    //    if (temp.x > 0)
    //        currentAnimation = moveRight;
    //    else
    //        currentAnimation = moveLeft;
    //    break;
    case SMOKEATTACK:
        if (temp.x > 0)
            currentAnimation = attackRight;
        else
            currentAnimation = attackLeft;

        if (currentAnimation->GetAttack() && m_bAttackAnimation)
        {
            float scythespeed = 1.5f;
            for (int i = 0; i < 4; ++i)
            {
                if (m_bSmokeActive[i])
                    scythespeed -= 0.2f;
            }
            for (int i = 0; i < 4; ++i)
            {
                if (m_bSmokeActive[i])
                {
                    Vector3 tempPos = smoke[i]->GetPosition();

                    temp = CPlayerInfo::GetInstance()->GetPosition() - tempPos;

                    CScythe* scythe = Create::Scythe(
                        tempPos,
                        temp,
                        scythespeed);
                }
            }

            m_bAttackAnimation = false;
        }
        if (!currentAnimation->GetAttack())
            m_bAttackAnimation = true;

        break;
    case SCYTHEATTACK:
        currentAnimation->SetCurrentFrame(0);

        if (temp.x > 0)
            currentAnimation = attackRight;
        else
            currentAnimation = attackLeft;

        if (currentAnimation->GetAttack() && m_bAttackAnimation)
        {
            temp = CPlayerInfo::GetInstance()->GetPosition() - position;

            CScythe* scythe = Create::Scythe(
                position,
                temp,
                5.0f,
                0.5f,
                true);

            m_bAttackAnimation = false;
        }
        if (!currentAnimation->GetAttack())
            m_bAttackAnimation = true;

        break;
    case DEAD:
        if (temp.x > 0)
            currentAnimation = dieRight;
        else
            currentAnimation = dieLeft;
        break;
    }

    if (currentAnimation)
    {
        currentAnimation->m_anim->animActive = true;
        if (fsm == FSM::MOVE)
            currentAnimation->Update(dt * 10.f);
        else if (fsm == FSM::SCYTHEATTACK)
            currentAnimation->Update(dt * 5.f);
        else
            currentAnimation->Update(dt * 1.f);

        if (fsm == FSM::DEAD && currentAnimation->GetCurrentFrame() == currentAnimation->m_anim->endFrame)
        {
            SetIsDone(true);
            return;
        }
    }

    if (prevHP != health)
    {
        prevHP = health;
        HPScale = ((float)health / 100.f) * 100.f;
    }

    Enemy2D::Update(dt);
}

void Reaper::Render(float& _renderOrder)
{
    if (!m_bInvulnerable)
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