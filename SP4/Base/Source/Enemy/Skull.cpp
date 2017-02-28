#include "Skull.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "LoadTGA.h"

Skull::Skull(const int _roomID)
    : GenericEntity(NULL)
    , m_dSpeed(30.0)
    , m_dResponseTime(0.0)
    , m_bAttackAnimation(false)
    , m_bAlive(true)
{
    this->position = Vector3(100, 0, 50);
    this->scale = Vector3(30, 30, 0);
    this->velocity = Vector3(0, 0, 0);

    this->isDone = false;
    this->m_bCollider = true;
    this->m_bLaser = false;

    this->m_eEntityType = EntityBase::ENEMY;

    moveLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    moveLeft->textureID = LoadTGA("Image//Enemy//skull_moveleft.tga");
    moveLeft->m_anim = new Animation;
    moveLeft->m_anim->Set(0, 3, 1, 1.f, true);

    moveRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    moveRight->textureID = LoadTGA("Image//Enemy//skull_moveright.tga");
    moveRight->m_anim = new Animation;
    moveRight->m_anim->Set(0, 3, 1, 1.f, true);

    attackLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    attackLeft->textureID = LoadTGA("Image//Enemy//skull_attackleft.tga");
    attackLeft->m_anim = new Animation;
    attackLeft->m_anim->Set(0, 3, 1, 1.f, true);
    attackLeft->SetAttackFrame(3);

    attackRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    attackRight->textureID = LoadTGA("Image//Enemy//skull_attackright.tga");
    attackRight->m_anim = new Animation;
    attackRight->m_anim->Set(0, 3, 1, 1.f, true);
    attackRight->SetAttackFrame(3);

    dieLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
    dieLeft->textureID = LoadTGA("Image//Enemy//skull_dieleft.tga");
    dieLeft->m_anim = new Animation;
    dieLeft->m_anim->Set(0, 6, 1, 1.f, true);

    dieRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
    dieRight->textureID = LoadTGA("Image//Enemy//skull_dieright.tga");
    dieRight->m_anim = new Animation;
    dieRight->m_anim->Set(0, 6, 1, 1.f, true);

    currentAnimation = moveLeft;

    HP = 15;
    roomID = _roomID;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 5; y = (y >> 1) - 5;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(this, roomID);
}

Skull::~Skull()
{
    delete moveLeft;
    delete moveRight;
    delete attackLeft;
    delete attackRight;
    delete dieLeft;
    delete dieRight;
}

void Skull::Update(double dt)
{
    CHeatmap** heatmap = CPlayerInfo::GetInstance()->GetHeatmap();

    int x = index.x;
    int z = index.z;

    velocity = heatmap[x][z].GetDir();

    m_dResponseTime += dt;

    if (HP <= 0)
    {
        fsm = FSM::DEAD;
    }
    else
    {
        if ((position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() <= 200
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
        position += velocity * dt * m_dSpeed;

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
            //if (secondaryWeapon)
            //    secondaryWeapon->Discharge(this->position, CPlayerInfo::GetInstance()->GetPosition(), CPlayerInfo::GetInstance());

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
}

void Skull::Render()
{
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(currentAnimation);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void Skull::Constrain()
{

}