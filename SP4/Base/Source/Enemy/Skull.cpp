#include "Skull.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "LoadTGA.h"

Skull::Skull(const int _roomID)
    : m_dSpeed(30.0)
    , m_dResponseTime(0.0)
    , m_bAttackAnimation(false)
    , m_bAlive(true)
{
    this->position = Vector3(100, 0, 50);
    this->scale = Vector3(30, 30, 0);
    this->velocity = Vector3(0, 0, 0);
	this->heatmapDir = Vector3(0, 0, 0);

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
	
	this->maxHealth = 50;
	this->health = this->maxHealth;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1) - 5; y = (y >> 1) - 5;
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

	roomID = _roomID;

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

	heatmapDir = heatmap[x][z].GetDir();
	velocity += heatmapDir;

    m_dResponseTime += dt;

    // length of this enemy to player
    Vector3 temp = CPlayerInfo::GetInstance()->GetPosition() - position;

    if (health <= 0)
        fsm = FSM::DEAD;
    else
    {
        if (temp.LengthSquared() <= 300
            && m_dResponseTime >= m_dResponse)
        {
            fsm = FSM::ATTACK;
            m_dResponseTime = 0.0;
        }
        else if (m_dResponseTime >= m_dResponse)
        {
            int rand = Math::RandIntMinMax(0, 10);

            if (rand > 2)
                fsm = FSM::MOVE;
            else
                fsm = FSM::CHASE;

            m_dResponseTime = 0.0;
        }
    }

    switch (fsm)
    {
    case MOVE:
        position += velocity * dt * m_dSpeed * 0.5f;

        if (temp.x > 0)
            currentAnimation = moveRight;
        else
            currentAnimation = moveLeft;
        break;
    case CHASE:
        position += velocity * dt * m_dSpeed * 2;

        if (temp.x > 0)
            currentAnimation = moveRight;
        else
            currentAnimation = moveLeft;
        break;
    case ATTACK:
        if (temp.x > 0)
            currentAnimation = attackRight;
        else
            currentAnimation = attackLeft;

        if (currentAnimation->GetAttack() && m_bAttackAnimation)
            m_bAttackAnimation = false;
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
        if (fsm == FSM::CHASE)
            currentAnimation->Update(dt * 6.0f);
        else if (fsm == FSM::ATTACK)
            currentAnimation->Update(dt * 2.0f);
        else if (fsm == FSM::DEAD)
            currentAnimation->Update(dt * 1.0f);
        else
            currentAnimation->Update(dt * 0.5f);

        if (fsm == FSM::DEAD && currentAnimation->GetCurrentFrame() == currentAnimation->m_anim->endFrame)
            SetIsDone(true);
    }

	Enemy2D::Update(dt);

	velocity.SetZero();
}

void Skull::Render(float& _renderOrder)
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

void Skull::Constrain()
{

}