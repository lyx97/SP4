#include "Enemy2D.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"

Enemy2D::Enemy2D()
	: GenericEntity(NULL)
{
	this->position = Vector3(0, 0, 0);

	this->isDone = false;
	this->m_bCollider = true;
	this->m_bLaser = false;
	this->nightmare = false;

	this->m_eEntityType = EntityBase::ENEMY;

	this->speed = 1.0f;
	this->maxHealth = 1;
	this->health = this->maxHealth;

	this->prevHealth = 0;
	this->healthScale = 0.0f;
	this->healthRatio = 0.0f;
	this->damage = 1.0f;

    this->scale = Vector3(20, 20, 1);
	int x = scale.x, y = scale.y;
	x = (x >> 1) - 5; y = (y >> 1) - 5;
	this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    // Particle
    this->color.Set(Vector3(1.f, 1.f, 1.f));
}

Enemy2D::~Enemy2D()
{
}

void Enemy2D::Update(double _dt)
{
	if (CPlayerInfo::GetInstance()->GetDreamBarRatio() < 0.25)
		nightmare = true;
	else
		nightmare = false;

	// Force y position 0
    position.y = 0;


	if (isDone)
	{
		CPlayerInfo::GetInstance()->SetDreamBar(CPlayerInfo::GetInstance()->GetDreamBar() + 2);
		float randomNo = Math::RandFloatMinMax(0, 100);
		if (randomNo < CHANCE_OF_DROPPING_POWERUP)
		{
			Powerup* newPowerup = new Powerup(this->position, CPlayerInfo::GetInstance()->GetRoomID());
			if (CPlayerInfo::GetInstance()->GetDreamBarRatio() < 0.75)
				newPowerup->Init();
		}
		if (CPlayerInfo::GetInstance()->GetTreasure()->treasure_type != 0 &&
			CPlayerInfo::GetInstance()->killCount < CPlayerInfo::GetInstance()->GetTreasure()->GetCooldown())
			CPlayerInfo::GetInstance()->killCount++;
	}

	if (this->prevHealth != health)
	{
		prevHealth = health;
		healthScale = ((float)health / maxHealth) * 30.f;
		healthRatio = health / maxHealth;
	}
}

void Enemy2D::Render(float& _renderOrder)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelStack.PushMatrix();
	modelStack.Translate(position.x - (30.f * 0.25f), 0.0f, position.z - 25.f);
	modelStack.Scale(0.5f, 0.5f, 1);
	modelStack.Translate(healthScale * 0.5f, 0, 0);
	modelStack.Scale(healthScale, 7.0f, 1);
	if (healthRatio < 0.25f)
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_quad"));
	else if (healthRatio < 0.5f)
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_half"));
	else
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("health_full"));
	modelStack.PopMatrix();
}

Vector3 Enemy2D::Cohesion(Enemy2D* enemy)
{
	Vector3 result(0, 0, 0);
	Vector3 centreOfMass(0, 0, 0);
	int neighbour = 0;

	for (auto otherEnemy : EntityManager::GetInstance()->GetEntityList())
	{
		if (otherEnemy->GetEntityType() == EntityBase::ENEMY)
		{
			if (otherEnemy != enemy)
			{
				centreOfMass += otherEnemy->GetPosition();
				neighbour++;
			}
		}
	}

	if (neighbour > 0)
	{
		centreOfMass.x /= neighbour;
		centreOfMass.z /= neighbour;
		result = centreOfMass - enemy->position;
	}

	return result;
}

Vector3 Enemy2D::Separation(Enemy2D* enemy)
{
	Vector3 result(0, 0, 0);

	for (auto otherEnemy : EntityManager::GetInstance()->GetEntityList())
	{
		if (otherEnemy->GetEntityType() == EntityBase::ENEMY)
		{
			if (otherEnemy != enemy)
			{
				Vector3 headingVector = enemy->GetPosition() - otherEnemy->GetPosition();

				float scale = headingVector.LengthSquared() * 0.0001f;

				result = headingVector / scale;
			}
		}
	}

	return result;
}

Vector3 Enemy2D::Alignment(Enemy2D* enemy)
{
	Vector3 result(0, 0, 0);
	int neighbour = 0;

	for (auto otherEnemy : EntityManager::GetInstance()->GetEntityList())
	{
		if (otherEnemy->GetEntityType() == EntityBase::ENEMY)
		{
			if (otherEnemy != enemy)
			{
				result += otherEnemy->GetVelocity();
				neighbour++;
			}
		}
	}

	if (neighbour > 0)
	{
		result.x /= neighbour;
		result.z /= neighbour;
	}

	return result;
}
