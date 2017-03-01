#include "Spitter.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "../Level/Level.h"
#include "../Projectile/Spit.h"
#include "LoadTGA.h"

Spitter::Spitter(const int _roomID)
    : targetPos(Vector3(0, 0, 0))
    , m_dSpeed(30.0)
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
	this->damage = 5.f;

    this->m_eEntityType = EntityBase::ENEMY;

    idleLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    idleLeft->textureID = LoadTGA("Image//Enemy//spitter_idleleft.tga");
    idleLeft->m_anim = new Animation;
    idleLeft->m_anim->Set(0, 3, 1, 1.f, true);

    idleRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 4);
    idleRight->textureID = LoadTGA("Image//Enemy//spitter_idleright.tga");
    idleRight->m_anim = new Animation;
    idleRight->m_anim->Set(0, 3, 1, 1.f, true);

    moveLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 9);
    moveLeft->textureID = LoadTGA("Image//Enemy//spitter_moveleft.tga");
    moveLeft->m_anim = new Animation;
    moveLeft->m_anim->Set(0, 8, 1, 1.f, true);

    moveRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 9);
    moveRight->textureID = LoadTGA("Image//Enemy//spitter_moveright.tga");
    moveRight->m_anim = new Animation;
    moveRight->m_anim->Set(0, 8, 1, 1.f, true);

    attackLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 14);
    attackLeft->textureID = LoadTGA("Image//Enemy//spitter_attackleft.tga");
    attackLeft->m_anim = new Animation;
    attackLeft->m_anim->Set(0, 13, 1, 2.0f, true);
    attackLeft->SetAttackFrame(11);

    attackRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 14);
    attackRight->textureID = LoadTGA("Image//Enemy//spitter_attackright.tga");
    attackRight->m_anim = new Animation;
    attackRight->m_anim->Set(0, 13, 1, 2.0f, true);
    attackRight->SetAttackFrame(11);

    dieLeft = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 8);
    dieLeft->textureID = LoadTGA("Image//Enemy//spitter_dieleft.tga");
    dieLeft->m_anim = new Animation;
    dieLeft->m_anim->Set(0, 7, 1, 1.f, true);

    dieRight = MeshBuilder::GetInstance()->GenerateSpriteAnimation(1, 8);
    dieRight->textureID = LoadTGA("Image//Enemy//spitter_dieright.tga");
    dieRight->m_anim = new Animation;
    dieRight->m_anim->Set(0, 7, 1, 1.f, true);

    currentAnimation = moveLeft;

	maxHealth = 75;
	health = maxHealth;

    roomID = _roomID;

    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Boundary
    CRoom* room = CLevel::GetInstance()->GetRoom(roomID);
    int xSize = room->GetRoomXMax();
    int zSize = room->GetRoomZMax();
    minBoundary.Set(room->GetSpatialPartition()->GetGridPos(1, 1) - Vector3(GRIDSIZE, 0, GRIDSIZE));
    maxBoundary.Set(room->GetSpatialPartition()->GetGridPos(xSize - 1, zSize - 1) + Vector3(GRIDSIZE, 0, GRIDSIZE));
}

Spitter::~Spitter()
{
    delete idleLeft;
    delete idleRight;
    delete moveLeft;
    delete moveRight;
    delete attackLeft;
    delete attackRight;
    delete dieLeft;
    delete dieRight;
}

void Spitter::Update(double dt)
{
    CHeatmap** heatmap = CPlayerInfo::GetInstance()->GetHeatmap();

    int x = index.x;
    int z = index.z;

    //velocity = heatmap[x][z].GetDir();

    m_dResponseTime += dt;

    //fsm = FSM::LUNGE;
    //if (targetPos.IsZero())
    //{
    //    targetPos = CPlayerInfo::GetInstance()->GetPosition();
    //    velocity = (targetPos - position);
    //}

    //cout << velocity << endl;

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
        if ((position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() <= 40000
            && m_dResponseTime >= m_dResponse)
        {
            fsm = FSM::ATTACK;
            m_dResponseTime = 0.0;
        }
        else if (m_dResponseTime >= m_dResponse)
        {
            fsm = FSM::IDLE;
            m_dResponseTime = 0.0;
        }
    }

    // length of this enemy to player
    Vector3 temp = CPlayerInfo::GetInstance()->GetPosition() - position;

    switch (fsm)
    {
    case IDLE:
    {
        if (temp.x > 0)
            currentAnimation = idleRight;
        else
            currentAnimation = idleLeft;

        break;
    }
    case LUNGE:
    {
        position += velocity.Normalized() * dt * m_dSpeed;

        if (temp.x > 0)
            currentAnimation = moveRight;
        else
            currentAnimation = moveLeft;
        break;
    }
    case ATTACK:
    {
        if (temp.x > 0)
            currentAnimation = attackRight;
        else
            currentAnimation = attackLeft;

        if (currentAnimation->GetAttack() && m_bAttackAnimation)
        {
            CSpit* spit = Create::Spit(
                position,
                temp,
                170.f,
				damage);

            m_bAttackAnimation = false;
        }
        if (!currentAnimation->GetAttack())
            m_bAttackAnimation = true;

        break;
    }
    case DEAD:
    {
        if (temp.x > 0)
            currentAnimation = dieRight;
        else
            currentAnimation = dieLeft;
        break;
    }
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
}

void Spitter::Render(float& _renderOrder)
{
	Enemy2D::Render(_renderOrder);

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y +_renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(currentAnimation);
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void Spitter::Constrain()
{
    // Constrain player within the boundary
    if (position.x > maxBoundary.x - 1.0f)
    {
        position.x = maxBoundary.x - 1.0f;
        velocity.x = 0;
    }
    //if (position.y > maxBoundary.y - 1.0f)
    //	position.y = maxBoundary.y - 1.0f;
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
    //if (position.y < minBoundary.y + 1.0f)
    //	position.y = minBoundary.y + 1.0f;
    if (position.z < minBoundary.z + 1.0f)
    {
        position.z = minBoundary.z + 1.0f;
        velocity.z = 0;
    }
}