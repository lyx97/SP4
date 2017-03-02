#include "Treasure.h"

Treasure::Treasure()
    : GenericEntity(NULL)
    , treasure_type(NONE)
    , duration(0)
    , cooldown(0)
{
}

Treasure::Treasure(const int& _roomID)
	: GenericEntity(NULL)
	, treasure_type(NONE)
	, duration(0)
	, cooldown(0)
{
    this->roomID = _roomID;	

    this->position.Set(0, 0, 0);
	this->scale.Set(25, 25, 10);

	this->m_eEntityType = EntityBase::TREASURE;
	this->m_bLaser = false;
	this->m_bCollider = true;

	random = Math::RandIntMinMax(1, NUM_TREASURE - 1);

    EntityManager::GetInstance()->AddEntity(this, roomID);
}

Treasure::~Treasure()
{
}

void Treasure::SpawnTreasure(Vector3 pos, unsigned int type, const int& _roomID)
{
    this->roomID = _roomID;
	this->position.Set(pos);
    this->scale.Set(25, 25, 10);

    this->m_eEntityType = EntityBase::TREASURE;
    this->m_bLaser = false;
    this->m_bCollider = true;

    random = type;

    EntityManager::GetInstance()->AddEntity(this, roomID);
}

void Treasure::Effects()
{
	switch (treasure_type)
	{
	case Treasure::NONE:
	{
	}
	break;
	case Treasure::RAPID_HEALTHREGEN:
	{
		CPlayerInfo::GetInstance()->SetHealthRegen(CPlayerInfo::GetInstance()->GetHealthRegen() * 0.1f);
	}
	break;
	case Treasure::SPRINT:
	{
		CPlayerInfo::GetInstance()->SetMaxSpeed(CPlayerInfo::GetInstance()->GetMaxSpeed() * 1.5f);
	}
	break;
	case Treasure::DOUBLE_DAMAGE:
	{
		CPlayerInfo::GetInstance()->SetDamage(CPlayerInfo::GetInstance()->GetDamage() * 2);
	}
	break;
	case Treasure::INVINCIBLE:
	{
		CPlayerInfo::GetInstance()->SetIsInvincible(true);
	}
	break;
	case Treasure::INCREASE_FIRERATE:
	{
		CPlayerInfo::GetInstance()->SetTimeBetweenShots(CPlayerInfo::GetInstance()->GetTimeBetweenShots() * 0.1f);
	}
	break;
	} // end of switch
}

void Treasure::Render(float& _renderOrder)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("treasure_texture"));

    modelStack.PopMatrix();
}

void Treasure::SetValues()
{
	this->random = treasure_type;
	// Setting the cooldowns and duration of the treasures
	switch (treasure_type)
	{
		case Treasure::NONE:
		{
			cooldown = 0;
			duration = 0;
			name = "None";
		}
		break;
		case Treasure::RAPID_HEALTHREGEN:
		{
			cooldown = 15;
			duration = 5;
			name = "RAPID HEALTHREGENERATION";
		}
		break;
		case Treasure::SPRINT:
		{
			cooldown = 25;
			duration = 20;
			name = "SPRINT";
		}
		break;
		case Treasure::DOUBLE_DAMAGE:
		{
			cooldown = 40;
			duration = 7;
			name = "DOUBLE DAMAGE";
		}
		break;
		case Treasure::INVINCIBLE:
		{
			cooldown = 30;
			duration = 3;
			name = "INVINCIBLE";
		}
		break;
		case Treasure::INCREASE_FIRERATE:
		{
			cooldown = 20;
			duration = 7;
			name = "RAPID FIRE";
		}
		break;
	}
}
