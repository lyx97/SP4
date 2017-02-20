#include "Powerup.h"
#include "../PlayerInfo/PlayerInfo.h"

Powerup::Powerup()
	: GenericEntity(NULL)
{
}

Powerup::Powerup(Vector3 pos)
	: GenericEntity(NULL)
{
	EntityManager::GetInstance()->AddEntity(this);
	this->position.Set(pos);
	this->velocity.Set(0, 0, 0);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::ITEM;
	this->m_bLaser = false;
	this->m_bCollider = true;
}

Powerup::~Powerup()
{
}

void Powerup::Init()
{

}

void Powerup::Update(double _dt)
{
	if ((this->position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() < 100)
	{
		int random = Math::RandIntMinMax(0, NUM_POWERUP - 1);
		switch (random)
		{
		case Powerup::HEALTH_RECOVER:
		{
			cout << "Recover Health" << endl;
			CPlayerInfo::GetInstance()->RecoverHealth();
		}
		break;
		case Powerup::HEALTH_INCREASE:
		{
			cout << "Increase Max Health" << endl;
			CPlayerInfo::GetInstance()->SetMaxHealth(CPlayerInfo::GetInstance()->GetMaxHealth() + 5);
		}
		break;
		case Powerup::SPEED_INCREASE:
		{
			cout << "Increase Speed" << endl;
			CPlayerInfo::GetInstance()->SetMaxSpeed(CPlayerInfo::GetInstance()->GetMaxSpeed() + 10);
		}
		break;
		case Powerup::HEALTHREGEN_INCREASE:
		{
			cout << "Increase Health Regen" << endl;
			CPlayerInfo::GetInstance()->SetHealthRegen(CPlayerInfo::GetInstance()->GetHealthRegen() + 1);
		}
		break;
		default:
			break;
		}
		this->SetIsDone(true);
		return;
	}
}

void Powerup::Render()
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
