#include "Items.h"
#include "../PlayerInfo/PlayerInfo.h"

Items::Items()
	: GenericEntity(NULL)
{
	EntityManager::GetInstance()->AddEntity(this, true);
	this->position.Set(10, 0, 10);
	this->velocity.Set(0, 0, 0);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::ITEM;
	this->m_bLaser = false;
	this->m_bCollider = true;
}

Items::~Items()
{
}

void Items::Init()
{
}

void Items::Update(double _dt)
{
	if ((this->position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() < 100)
	{
		int random = Math::RandIntMinMax(0, NUM_POWERUP - 1);
		switch (random)
		{
		case Items::HEALTH_RECOVER:
		{
			CPlayerInfo::GetInstance()->SetHealth(CPlayerInfo::GetInstance()->GetMaxHealth());
		}
			break;
		case Items::HEALTH_INCREASE:
		{
			CPlayerInfo::GetInstance()->SetMaxHealth(CPlayerInfo::GetInstance()->GetMaxHealth() + 10);
		}
			break;
		case Items::SPEED_INCREASE:
		{
			cout << "increase speed" << endl;
		}
			break;
		default:
			break;
		}
		this->SetIsDone(true);
		return;
	}
}

void Items::Render()
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
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("lightball"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
}