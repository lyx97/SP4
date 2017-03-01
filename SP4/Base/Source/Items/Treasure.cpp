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
	this->scale.Set(10, 10, 10);

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
    this->scale.Set(10, 10, 10);

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
		cout << "NO TREASURE" << endl;
	}
	break;
	case Treasure::RAPID_HEALTHREGEN:
	{
		cout << "RAPID HEALTHREGEN USED" << endl;
		CPlayerInfo::GetInstance()->SetHealthRegen(CPlayerInfo::GetInstance()->GetHealthRegen() * 0.1f);
	}
	break;
	case Treasure::SPRINT:
	{
		cout << "SPRINT USED" << endl;
		CPlayerInfo::GetInstance()->SetMaxSpeed(CPlayerInfo::GetInstance()->GetMaxSpeed() * 1.5f);
	}
	break;
	case Treasure::ONE_HIT_KILL:
	{
		cout << "ONE SHOT KILL USED" << endl;
		CPlayerInfo::GetInstance()->SetDamage(9999);
	}
	break;
	case Treasure::INVINCIBLE:
	{
		cout << "INVINCIBLE USED" << endl;
		CPlayerInfo::GetInstance()->SetIsInvincible(true);
	}
	break;
	case Treasure::INCREASE_FIRERATE:
	{
		cout << "INCREASE_FIRERATE USED" << endl;
		CPlayerInfo::GetInstance()->SetTimeBetweenShots(CPlayerInfo::GetInstance()->GetTimeBetweenShots() * 0.5f);
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
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("lightball"));

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
			name = "Rapid Healthregen";
		}
		break;
		case Treasure::SPRINT:
		{
			cout << "SPRINT" << endl;
			cooldown = 25;
			duration = 20;
			name = "Sprint";
		}
		break;
		case Treasure::ONE_HIT_KILL:
		{
			cooldown = 40;
			duration = 7;
			name = "One hit kill";
		}
		break;
		case Treasure::INVINCIBLE:
		{
			cooldown = 30;
			duration = 3;
			name = "Invincibility";
		}
		break;
		case Treasure::INCREASE_FIRERATE:
		{
			cooldown = 20;
			duration = 7;
			name = "Rapidfire";
		}
		break;
	}
}
