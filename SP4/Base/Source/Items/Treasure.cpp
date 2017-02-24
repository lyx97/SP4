#include "Treasure.h"

Treasure::Treasure()
	: GenericEntity(NULL)
	, treasure(NONE)
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

	this->m_eEntityType = EntityBase::ITEM;
	this->m_bLaser = false;
	this->m_bCollider = true;

	random = Math::RandIntMinMax(1, NUM_TREASURE - 1);
}

void Treasure::SpawnTreasure(Vector3 pos, TREASURES type)
{
	EntityManager::GetInstance()->AddEntity(this, CPlayerInfo::GetInstance()->GetRoomID());

	this->position.Set(pos);
	this->scale.Set(10, 10, 10);

	this->m_eEntityType = EntityBase::ITEM;
	this->m_bLaser = false;
	this->m_bCollider = true;

	random = type;
}

void Treasure::Update(double _dt)
{
	if ((this->position - CPlayerInfo::GetInstance()->GetPosition()).LengthSquared() < 100 &&
		KeyboardController::GetInstance()->IsKeyPressed('Q'))
	{
		CPlayerInfo::GetInstance()->DropTreasure();
		switch (random)
		{
			case Treasure::RAPID_HEALTHREGEN:
			{
				cout << "RAPID HEALTH REGEN" << endl;
				CPlayerInfo::GetInstance()->AddTreasures(RAPID_HEALTHREGEN);
			}
			break;
			case Treasure::SPRINT:
			{
				cout << "SPRINT" << endl;
				CPlayerInfo::GetInstance()->AddTreasures(SPRINT);
			}
			break;
			case Treasure::ONE_HIT_KILL:
			{
				cout << "ONE SHOT KILL" << endl;
				CPlayerInfo::GetInstance()->AddTreasures(ONE_HIT_KILL);
			}
			break;
			case Treasure::INVINCIBLE:
			{
				cout << "INVINCIBLE" << endl;
				CPlayerInfo::GetInstance()->AddTreasures(INVINCIBLE);
			}
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

void Treasure::SetValues()
{
	this->random = treasure;
	// Setting the cooldowns and duration of the treasures
	switch (treasure)
	{
		case Treasure::NONE:
		{
			cooldown = 0;
			duration = 0;
		}
		break;
		case Treasure::RAPID_HEALTHREGEN:
		{
			cooldown = 15;
			duration = 5;
		}
		break;
		case Treasure::SPRINT:
		{
			cooldown = 25;
			duration = 20;
		}
		break;
		case Treasure::ONE_HIT_KILL:
		{
			cooldown = 25;
			duration = 7;
		}
		case Treasure::INVINCIBLE:
		{
			cooldown = 30;
			duration = 3;
		}
		break;
	}
}
