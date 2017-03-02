#include "Skull.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "LoadTGA.h"
#include "../Sound/SoundManager.h"

Skull::Skull(const int _roomID)
    : m_dSpeed(30.0)
    , m_dResponseTime(0.0)
    , m_bAttackAnimation(false)
    , m_bAlive(true)
	, transformed(false)
{
    this->position = Vector3(100, 0, 50);
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

	transformingLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
	transformingLeft->textureID = LoadTGA("Image//Enemy//skull_transformingleft.tga");
	transformingLeft->m_anim = new Animation;
	transformingLeft->m_anim->Set(0, 6, 1, 1.f, true);

	transformingRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
	transformingRight->textureID = LoadTGA("Image//Enemy//skull_transformingright.tga");
	transformingRight->m_anim = new Animation;
	transformingRight->m_anim->Set(0, 6, 1, 1.f, true);

	untransformingLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
	untransformingLeft->textureID = LoadTGA("Image//Enemy//skull_untransformingleft.tga");
	untransformingLeft->m_anim = new Animation;
	untransformingLeft->m_anim->Set(0, 6, 1, 1.f, true);

	untransformingRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 7);
	untransformingRight->textureID = LoadTGA("Image//Enemy//skull_untransformingright.tga");
	untransformingRight->m_anim = new Animation;
	untransformingRight->m_anim->Set(0, 6, 1, 1.f, true);

	transformedLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 2);
	transformedLeft->textureID = LoadTGA("Image//Enemy//skull_transformedleft.tga");
	transformedLeft->m_anim = new Animation;
	transformedLeft->m_anim->Set(0, 1, 1, 1.f, true);

	transformedRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 2);
	transformedRight->textureID = LoadTGA("Image//Enemy//skull_transformedright.tga");
	transformedRight->m_anim = new Animation;
	transformedRight->m_anim->Set(0, 1, 1, 1.f, true);

    currentAnimation = moveLeft;
	
	maxHealth = 75;
	health = maxHealth;

	roomID = _roomID;

	EntityManager::GetInstance()->AddEntity(this, roomID);

    // Boundary
    minBoundary.Set(CPlayerInfo::GetInstance()->GetMinBoundary());
    maxBoundary.Set(CPlayerInfo::GetInstance()->GetMaxBoundary());
}

Skull::~Skull()
{
    delete moveLeft;
    delete moveRight;
    delete attackLeft;
    delete attackRight;
    delete dieLeft;
    delete dieRight;
	delete transformingLeft;
	delete transformingRight;
	delete untransformingLeft;
	delete untransformingRight;
	delete transformedLeft;
	delete transformedRight;
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
			SoundManager::GetInstance()->PlaySoundEffect2D("Sound//whoosh.wav");
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
	if (nightmare && !transformed)
		fsm = FSM::TRANSFORMING;

	if (fsm == FSM::TRANSFORMING && currentAnimation->GetCurrentFrame() == currentAnimation->m_anim->endFrame)
	{
		fsm = FSM::MOVE;
		transformed = true;
		m_dSpeed = 50.0f;
		damage = 10.f;
	}

	if (!nightmare && transformed)
		fsm = FSM::UNTRANSFORMING;

	if (fsm == FSM::UNTRANSFORMING && currentAnimation->GetCurrentFrame() == currentAnimation->m_anim->endFrame)
	{
		fsm = FSM::MOVE;
		transformed = false;
		m_dSpeed = 30.0f;
		damage = 5.f;
	}

    switch (fsm)
    {
    case MOVE:
        position += velocity * dt * m_dSpeed * 0.5;
		if (!transformed)
		{
			if (temp.x > 0)
				currentAnimation = moveRight;
			else
				currentAnimation = moveLeft;
		}
		else
		{
			if (temp.x > 0)
				currentAnimation = transformedRight;
			else
				currentAnimation = transformedLeft;
		}
        break;
    case CHASE:
        position += velocity * dt * m_dSpeed * 2.0;
		if (!transformed)
		{
			if (temp.x > 0)
				currentAnimation = moveRight;
			else
				currentAnimation = moveLeft;
		}
		else
		{
			if (temp.x > 0)
				currentAnimation = transformedRight;
			else
				currentAnimation = transformedLeft;
		}
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
	case TRANSFORMING:
		if (temp.x > 0)
			currentAnimation = transformingRight;
		else
			currentAnimation = transformingLeft;
		break;
	case UNTRANSFORMING:
		if (temp.x > 0)
			currentAnimation = untransformingRight;
		else
			currentAnimation = untransformingLeft;
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
		else if (fsm == FSM::TRANSFORMING)
			currentAnimation->Update(dt * 2.0f);
		else if (fsm == FSM::UNTRANSFORMING)
			currentAnimation->Update(dt * 2.0f);
		else if (fsm == FSM::TRANSFORMED)
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