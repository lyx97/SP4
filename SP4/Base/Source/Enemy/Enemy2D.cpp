#include "Enemy2D.h"


Enemy2D::Enemy2D()
	: GenericEntity(NULL)
{
	EntityManager::GetInstance()->AddEntity(this, true);
	this->position = Vector3(0, 0, 0);
	this->scale = Vector3(1, 3, 1);

	this->isDone = false;
	this->m_bCollider = true;
	this->m_bLaser = false;
	this->m_bActive = false;

	this->m_eEntityType = EntityBase::ENEMY;

	this->velocity.Set(
		Math::RandFloatMinMax(-100, 100),
		0,
		Math::RandFloatMinMax(-100, 100));

	this->speed = 1.0f;
	this->scale.Set(10, 10, 10);
}

Enemy2D::~Enemy2D()
{
}

void Enemy2D::Init()
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
	for (auto proj : EntityManager::GetInstance()->GetEntityList())
	{
		if (proj->GetEntityType() == EntityBase::PROJECTILE)
		{
			if ((proj->GetPosition() - this->position).LengthSquared() < 200)
			{
				proj->SetIsDone(true);
				this->velocity += proj->GetVelocity() * proj->GetMass();;
			}
		}
	}
	Vector3 temp = (Cohesion(this) + Alignment(this) + Separation(this));
	if (!temp.IsZero())
	{
		this->velocity += temp.Normalized();
	}

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
				if ((otherEnemy->GetPosition() - this->GetPosition()).LengthSquared() < 10000)
				{
					Vector3 headingVector = enemy->GetPosition() - otherEnemy->GetPosition();

					float scale = headingVector.LengthSquared() * 0.001f;

					result = headingVector / scale;
				}
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

void Enemy2D::Render()
{
	GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	GraphicsManager::GetInstance()->GetModelStack().Translate(
		this->position.x,
		this->position.y,
		this->position.z);
	GraphicsManager::GetInstance()->GetModelStack().Scale(
		this->scale.x,
		this->scale.y,
		this->scale.z);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
}
