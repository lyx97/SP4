#include "Treasure.h"

Treasure::Treasure()
	: GenericEntity(NULL)
	, treasure_type(NONE)
	, duration(0)
	, cooldown(0)
{
}

Treasure::~Treasure()
{
}

void Treasure::Init()
{
	EntityManager::GetInstance()->AddEntity(this, CPlayerInfo::GetInstance()->GetRoomID());

	this->position.Set(0, 0, 0);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::TREASURE;
	this->m_bLaser = false;
	this->m_bCollider = true;

	random = Math::RandIntMinMax(1, NUM_TREASURE - 1);
}

void Treasure::SpawnTreasure(Vector3 pos, unsigned int type)
{
	EntityManager::GetInstance()->AddEntity(this, CPlayerInfo::GetInstance()->GetRoomID());

	this->position.Set(pos);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::TREASURE;
	this->m_bLaser = false;
	this->m_bCollider = true;

	random = type;
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
	}
	break;
	case Treasure::INVINCIBLE:
	{
		cout << "INVINCIBLE USED" << endl;
	}
	break;
	} // end of switch
}

void Treasure::Render()
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
		}
		break;
		case Treasure::RAPID_HEALTHREGEN:
		{
			cout << "RAPID_HEALTHREGEN" << endl;
			cooldown = 15;
			duration = 5;
		}
		break;
		case Treasure::SPRINT:
		{
			cout << "SPRINT" << endl;
			cooldown = 25;
			duration = 20;
		}
		break;
		case Treasure::ONE_HIT_KILL:
		{
			cout << "ONE_HIT_KILL" << endl;
			cooldown = 25;
			duration = 7;
		}
		case Treasure::INVINCIBLE:
		{
			cout << "INVINCIBLE" << endl;
			cooldown = 30;
			duration = 3;
		}
		break;
	}
}
