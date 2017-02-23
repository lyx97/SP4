#include "Treasure.h"

Treasure::Treasure()
	: GenericEntity(NULL)
{
	EntityManager::GetInstance()->AddEntity(this);

	this->position.Set(0, 0, 0);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::ITEM;
	this->m_bLaser = false;
	this->m_bCollider = true;

	cooldown = 0;

	treasure = NONE;

	random = Math::RandIntMinMax(0, NUM_TREASURE - 1);
}

Treasure::Treasure(int type)
	: GenericEntity(NULL)
{
	EntityManager::GetInstance()->AddEntity(this);

	this->position.Set(0, 0, 0);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::ITEM;
	this->m_bLaser = false;
	this->m_bCollider = true;

	cooldown = 0;

	treasure = NONE;

	random = Math::RandIntMinMax(0, NUM_TREASURE - 1);
}

Treasure::~Treasure()
{
}

void Treasure::Init()
{
	switch (treasure)
	{
	case Treasure::NONE:
	{
		cout << "NO TREASURE" << endl;
	}
	break;
	case Treasure::HEALTH_INCREASE:
	{	
		cout << "Increase health CD 10" << endl;
		cooldown = 10;
	}
	break;
	case Treasure::SPEED_INCREASE:
	{
		cout << "Increase Speed CD 15" << endl;
		cooldown = 15;
	}
	break;
	case Treasure::HEALTHREGEN_INCREASE:
	{
		cout << "Increase Health Regen CD 20" << endl;		
		cooldown = 20;
	}
	break;
	default:
		break;
	}
}

void Treasure::Update(double _dt)
{
	if ((this->position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() < 100)
	{
		switch (random)
		{
		case Treasure::HEALTH_INCREASE:
		{
			cout << "Increase Health" << endl;
			CPlayerInfo::GetInstance()->AddTreasures(HEALTH_INCREASE);
		}
		break;
		case Treasure::SPEED_INCREASE:
		{
			cout << "Increase Speed" << endl;
			CPlayerInfo::GetInstance()->AddTreasures(SPEED_INCREASE);
		}
		break;
		case Treasure::HEALTHREGEN_INCREASE:
		{
			cout << "Increase Health Regen" << endl;
			CPlayerInfo::GetInstance()->AddTreasures(HEALTHREGEN_INCREASE);
		}
		break;
		default:
			break;
		}
		this->SetIsDone(true);
		return;
	}
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