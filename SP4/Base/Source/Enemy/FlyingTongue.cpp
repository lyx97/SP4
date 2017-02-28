#include "FlyingTongue.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "../Projectile/Ghast.h"
#include "LoadTGA.h"

FlyingTongue::FlyingTongue(const int _roomID)
    : m_dSpeed(30.0)
    , m_dResponseTime(0.0)
    , m_bAttackAnimation(false)
    , m_bAlive(true)
{
    this->position = Vector3(100, 0, 0);
    this->scale = Vector3(30, 30, 0);
    this->velocity = Vector3(0, 0, 0);

    this->isDone = false;
    this->m_bCollider = true;
    this->m_bLaser = false;

    this->m_eEntityType = EntityBase::ENEMY;
    
    moveLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    moveLeft->textureID = LoadTGA("Image//Enemy//flyingtongue_moveleft.tga");
    moveLeft->m_anim = new Animation;
    moveLeft->m_anim->Set(0, 4, 1, 1.f, true);
    
    moveRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    moveRight->textureID = LoadTGA("Image//Enemy//flyingtongue_moveright.tga");
    moveRight->m_anim = new Animation;
    moveRight->m_anim->Set(0, 4, 1, 1.f, true);

    attackLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    attackLeft->textureID = LoadTGA("Image//Enemy//flyingtongue_attackleft.tga");
    attackLeft->m_anim = new Animation;
    attackLeft->m_anim->Set(0, 4, 1, 1.f, true);
    attackLeft->SetAttackFrame(2);

    attackRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 5);
    attackRight->textureID = LoadTGA("Image//Enemy//flyingtongue_attackright.tga");
    attackRight->m_anim = new Animation;
    attackRight->m_anim->Set(0, 4, 1, 1.f, true);
    attackRight->SetAttackFrame(2);

    dieLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 8);
    dieLeft->textureID = LoadTGA("Image//Enemy//flyingtongue_dieleft.tga");
    dieLeft->m_anim = new Animation;
    dieLeft->m_anim->Set(0, 8, 1, 1.f, true);

    dieRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 8);
    dieRight->textureID = LoadTGA("Image//Enemy//flyingtongue_dieright.tga");
    dieRight->m_anim = new Animation;
    dieRight->m_anim->Set(0, 8, 1, 1.f, true);

    currentAnimation = moveLeft;

	health = 5;
    roomID = _roomID;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 5; y = (y >> 1) - 5;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(this, roomID);
}

FlyingTongue::~FlyingTongue()
{
    delete moveLeft;
    delete moveRight;
    delete attackLeft;
    delete attackRight;
    delete dieLeft;
    delete dieRight;
}

void FlyingTongue::Update(double dt)
{
	Enemy2D::Update(dt);
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
        if ((position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() <= 10000
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
            CGhast* ghast = Create::Ghast(
                "ghast",
                position,
                CPlayerInfo::GetInstance()->GetPosition() - position,
                5.0f,
                30.0f);

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

void FlyingTongue::Render(float& _renderOrder)
{
	Enemy2D::Render(_renderOrder);

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(currentAnimation);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void FlyingTongue::Constrain()
{

}