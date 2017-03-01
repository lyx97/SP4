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
    this->heatmapDir = Vector3(0, 0, 0);
    
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

	maxHealth = 75;
	health = maxHealth;

    roomID = _roomID;

    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Boundary
    minBoundary.Set(CPlayerInfo::GetInstance()->GetMinBoundary());
    maxBoundary.Set(CPlayerInfo::GetInstance()->GetMaxBoundary());
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
    CHeatmap** heatmap = CPlayerInfo::GetInstance()->GetHeatmap();

    int x = index.x;
    int z = index.z;

    heatmapDir = heatmap[x][z].GetDir();
    velocity += heatmapDir;

    m_dResponseTime += dt;

    // length of this enemy to player
    Vector3 temp = CPlayerInfo::GetInstance()->GetPosition() - position;

	if (nightmare)
	{
		m_dSpeed = 50.0f;
		damage = 10.f;
	}
	else
	{
		m_dSpeed = 30.0f;
		damage = 5.f;
	}

    if (health <= 0)
    {
        fsm = FSM::DEAD;
    }
    else
    {
        if (temp.LengthSquared() <= 20000
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
        {
            CGhast* ghast = Create::Ghast(
                "ghast",
                position,
                CPlayerInfo::GetInstance()->GetPosition() - position,
                5.0f,
                30.0f,
				damage);

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
		if (nightmare)
			currentAnimation->Update(dt * 3.f);
		else
			currentAnimation->Update(dt * 1.f);

        if (fsm == FSM::DEAD && currentAnimation->GetCurrentFrame() == currentAnimation->m_anim->endFrame)
            SetIsDone(true);
    }

	Enemy2D::Update(dt);

    velocity.SetZero();
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
    // Constrain entity within the boundary
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