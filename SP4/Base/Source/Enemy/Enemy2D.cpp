#include "Enemy2D.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

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
	this->isLeader = false;
}

Enemy2D::~Enemy2D()
{
}

void Enemy2D::Init()
{
	this->scale = Vector3(1, 7, 1);
	this->isLeader = true;
}

void Enemy2D::Update(double _dt)
{
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

	if (!this->isLeader)
	{
		Vector3 alignment, cohesion, separation;

		// Alignment
		if (!this->velocity.IsZero())
		{
			
		}

		// Cohesion
		if (!this->velocity.IsZero())
		{
			cohesion = (leaderPos - this->position).Normalized();
			//cohesion = cohesion / neighbour;
		}

		// Separation
		if (!this->velocity.IsZero())
		{
			separation = (leaderPos + this->position).Normalized();
			separation.x *= -1;
			separation.z *= -1;
		}

		this->velocity += /*alignment*/  cohesion + separation;
	}
}

void Enemy2D::GoToRandomPos(Vector3 pos)
{
	this->leaderPos.Set(pos);
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
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cone"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();

	GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	GraphicsManager::GetInstance()->GetModelStack().Translate(
		leaderPos.x,
		leaderPos.y,
		leaderPos.z);
	GraphicsManager::GetInstance()->GetModelStack().Scale(
		3,
		3,
		3);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("lightball"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
}
