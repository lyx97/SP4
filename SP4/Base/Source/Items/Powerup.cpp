#include "Powerup.h"
#include "../PlayerInfo/PlayerInfo.h"

Powerup::Powerup(const int& _roomID)
	: GenericEntity(NULL)
{
	this->position.Set(0, 0, 0);
	this->velocity.Set(0, 0, 0);
	this->scale.Set(20, 20, 20);

	this->m_eEntityType = EntityBase::POWERUP;

	this->roomID = _roomID;

	EntityManager::GetInstance()->AddEntity(this, roomID);
}

Powerup::Powerup(Vector3 pos, const int& _roomID)
	: GenericEntity(NULL)
{
	this->position.Set(pos);
	this->velocity.Set(0, 0, 0);
	this->scale.Set(20, 20, 20);

	this->m_eEntityType = EntityBase::POWERUP;
	this->m_bLaser = false;
	this->m_bCollider = true;

	random = Math::RandIntMinMax(0, NUM_POWERUP - 1);

    this->roomID = _roomID;

    EntityManager::GetInstance()->AddEntity(this, roomID);
}

Powerup::~Powerup()
{
}

void Powerup::Init()
{
	random = HEALTH_RECOVER;
}

void Powerup::Update(double _dt)
{
	if ((this->position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() < 100)
	{
		switch (random)
		{
		case Powerup::HEALTH_RECOVER:
		{
			CPlayerInfo::GetInstance()->RecoverHealth();
		}
		break;
		case Powerup::HEALTH_INCREASE:
		{
			CPlayerInfo::GetInstance()->SetMaxHealth(CPlayerInfo::GetInstance()->GetMaxHealth() + 50);
		}
		break;
		case Powerup::SPEED_INCREASE:
		{
			CPlayerInfo::GetInstance()->SetMaxSpeed(CPlayerInfo::GetInstance()->GetMaxSpeed() + 50);
		}
		break;
		case Powerup::HEALTHREGEN_INCREASE:
		{
			CPlayerInfo::GetInstance()->SetHealthRegen(CPlayerInfo::GetInstance()->GetHealthRegen() + 5);
		}
		break;
		case Powerup::DAMAGE_INCREASE:
		{
			CPlayerInfo::GetInstance()->SetDamage(CPlayerInfo::GetInstance()->GetDamage() + 2);
		}
		break;
		} // end of break
		this->SetIsDone(true);
		return;
	}
}

void Powerup::Render(float& _renderOrder)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);

	switch (random)
	{
	case Powerup::HEALTH_RECOVER:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("powerup_healthrecover"));
	}
	break;
	case Powerup::HEALTH_INCREASE:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("powerup_maxhealth"));
	}
	break;
	case Powerup::SPEED_INCREASE:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("powerup_speed"));
	}
	break;
	case Powerup::HEALTHREGEN_INCREASE:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("powerup_healthregen"));
	}
	break;
	case Powerup::DAMAGE_INCREASE:
	{
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("powerup_healthregen"));
	}
	break;
	} // end of switch

	modelStack.PopMatrix();
}
