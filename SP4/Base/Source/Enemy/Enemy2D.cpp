#include "Enemy2D.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"

Enemy2D::Enemy2D()
	: GenericEntity(NULL)
{
	this->position = Vector3(0, 0, 0);
	this->scale = Vector3(1, 5, 1);

	this->isDone = false;
	this->m_bCollider = true;
	this->m_bLaser = false;

	this->m_eEntityType = EntityBase::ENEMY;

	this->velocity.Set(
		Math::RandFloatMinMax(-100, 100),
		0,
		Math::RandFloatMinMax(-100, 100));

	this->speed = 1.0f;
	this->maxHealth = 100;
	this->health = this->maxHealth;

    EntityManager::GetInstance()->AddEntity(this, CPlayerInfo::GetInstance()->GetRoomID());
}

Enemy2D::~Enemy2D()
{
}

void Enemy2D::Update(double _dt)
{
	if (this->velocity.LengthSquared() > ENEMY_MAX_SPEED * ENEMY_MAX_SPEED)
		velocity = velocity.Normalized() * ENEMY_MAX_SPEED;

	this->position += this->velocity * _dt * speed;

	if (this->position.x > Application::GetInstance().GetWindowWidth() * 0.5f)
	{
		this->position.x -= Application::GetInstance().GetWindowWidth();
	}
	else if (this->position.x < -Application::GetInstance().GetWindowWidth() * 0.5f)
	{
		this->position.x += Application::GetInstance().GetWindowWidth();
	}
	if (this->position.z > Application::GetInstance().GetWindowHeight() * 0.5f)
	{
		this->position.z -= Application::GetInstance().GetWindowHeight();
	}
	else if (this->position.z < -Application::GetInstance().GetWindowHeight() * 0.5f)
	{
		this->position.z += Application::GetInstance().GetWindowHeight();
	}
	//for (auto entity : EntityManager::GetInstance()->GetEntityList())
	//{
	//	if (entity->GetEntityType() == EntityBase::PROJECTILE)
	//	{
	//		CProjectile* proj = dynamic_cast<CProjectile*>(entity);
	//		if ((proj->GetPosition() - this->position).LengthSquared() < 200)
	//		{
	//			this->velocity += proj->GetVelocity() * proj->GetMass();
	//			this->health -= proj->GetDamage();
	//			//proj->SetIsDone(true);
	//			//this->SetIsDone(true); // debugging
	//		}
	//	}
	//}
	Vector3 temp = (Cohesion(this) +Alignment(this) + Separation(this));
	if (!temp.IsZero())
	{
		this->velocity += temp.Normalized();
	}
	if (this->health <= 0)
	{
		isDone = true;
	}
	if (isDone)
	{
		float randomNo = Math::RandFloatMinMax(0, 100);
		if (randomNo < CHANCE_OF_DROPPING_POWERUP)
		{
			Powerup* newPowerup = new Powerup(this->position, CPlayerInfo::GetInstance()->GetRoomID());
			newPowerup->Init();
		}
		if (CPlayerInfo::GetInstance()->GetTreasure()->treasure_type != 0 &&
			CPlayerInfo::GetInstance()->killCount < CPlayerInfo::GetInstance()->GetTreasure()->GetCooldown())
			CPlayerInfo::GetInstance()->killCount++;
	}
}

void Enemy2D::Render(float& _renderOrder)
{
	GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	GraphicsManager::GetInstance()->GetModelStack().Translate(
		this->position.x,
        this->position.y + _renderOrder,
		this->position.z);
	GraphicsManager::GetInstance()->GetModelStack().Scale(
		this->scale.x,
		this->scale.y,
		this->scale.z);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cone"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
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
