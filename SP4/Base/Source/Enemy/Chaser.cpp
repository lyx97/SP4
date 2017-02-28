#include "Chaser.h"

CChaser::CChaser()
	: GenericEntity(NULL)
	, speed(1)
	, hp(100)
	, goToPosition(0)
	, Chaser_State(PATROL)
	, counter(0.0f)
{
	EntityManager::GetInstance()->AddEntity(this, false);
	this->position = Vector3(50, 50, 50);
	this->scale = Vector3(1, 3, 1);

	this->isDone = false;
	this->m_bCollider = true;
	this->m_bLaser = false;

	this->m_eEntityType = EntityBase::ENEMY;

	this->velocity.Set(
		Math::RandFloatMinMax(-100, 100),
		0,
		Math::RandFloatMinMax(-100, 100));

	this->scale.Set(10, 10, 10);

	playerInfo = CPlayerInfo::GetInstance();
	ChaserWaypoint = CWaypointManager::GetInstance();

	// Create a Waypoint inside WaypointManager
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Chaser_A_1");
	ChaserPoint1 = CWaypointManager::GetInstance()->AddWaypoint(
		Vector3(
		CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")
		));

	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Chaser_A_2");
	ChaserPoint2 = CWaypointManager::GetInstance()->AddWaypoint(
		ChaserPoint1,
		Vector3(
		CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")
		));

	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Chaser_A_3");
	ChaserPoint3 = CWaypointManager::GetInstance()->AddWaypoint(
		ChaserPoint2,
		Vector3(
		CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")
		));

	CWaypointManager::GetInstance()->PrintSelf();
}

CChaser::~CChaser()
{
}

float GetDistance(float x1, float y1, float x2, float y2) { return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); } // OK

void CChaser::MovePos(Vector3 finalpos, float speed)
{
	float distance = GetDistance(this->GetPosition().x, this->GetPosition().z, finalpos.x, finalpos.z);
	//if (distance >= 0.6f)
	if (distance >= speed)
	{
		Vector3 currentPos;
		currentPos = Vector3(this->GetPosition().x, this->GetPosition().z, 1);
		Vector3 direction;
		direction = (finalpos - currentPos).Normalized();
		direction = Vector3(direction.x, direction.z, 0);
		this->SetPosition(currentPos + direction * speed);
	}
	else
	{
		this->SetPosition(finalpos);
	}
}

bool CChaser::ReachPos(Vector3 finalpos)
{
	float distance = GetDistance(this->GetPosition().x, this->GetPosition().z, finalpos.x, finalpos.z);
	if (distance < 0.6f)
	{
		return true;
	}
	return false;
}

void CChaser::RanPosition()
{
	goToPosition = rand() % 3;
	switch (goToPosition)
	{
	case 0:
		newChaserPos = ChaserWaypoint->GetWaypoint(ChaserPoint1)->GetPosition();
		break;
	case 1:
		newChaserPos = ChaserWaypoint->GetWaypoint(ChaserPoint2)->GetPosition();
		break;
	case 2:
		newChaserPos = ChaserWaypoint->GetWaypoint(ChaserPoint3)->GetPosition();
		break;
	}
}

void CChaser::FindEscapeRoute()
{
	EscapePos = ChaserWaypoint->GetNearestWaypoint(this->position)->GetPosition();
	if (DistanceSquaredBetween(EscapePos, playerInfo->GetPosition()) < 300)
	{
		EscapePos = ChaserWaypoint->GetNearestWaypoint(EscapePos)->GetPosition();
	}
}

void CChaser::Respond()
{
	RunFSM();
	switch (Chaser_State)
	{
	case PATROL:
		if (this->ReachPos(newChaserPos))
		{
			RanPosition();
		}
		else
		{
			this->MovePos(newChaserPos, 1);
		}
		break;

	case CHASE:
		this->MovePos(playerInfo->GetPosition(), 2);
		break;

	case ATTACK:
		//ChaserBlast->Discharge(this->position, playerInfo->GetPosition(), playerInfo);
		break;

	case ESCAPE:
		FindEscapeRoute();
		this->SetPosition(EscapePos);
		break;
	}
}

void CChaser::RunFSM()
{
	RanPosition();
	if (hp <= 40)
	{
		if (DistanceSquaredBetween(this->position, playerInfo->GetPosition()) < 100)
		{
			Chaser_State = ESCAPE;
		}
		else
		{
			Chaser_State = PATROL;
		}
	}

	else
	{
		if (DistanceSquaredBetween(this->position, playerInfo->GetPosition()) < 200)
		{
			Chaser_State = CHASE;
		}

		else if (DistanceSquaredBetween(this->position, playerInfo->GetPosition()) < 100)
		{
			Chaser_State = ATTACK;
		}
		else
		{
			Chaser_State = PATROL;
		}
	}
}

void CChaser::FSMUpdate()
{
	RunFSM();
	Respond();
}

void CChaser::Update(double dt)
{
	FSMUpdate();
	//cout << goToPosition << endl;
	switch (Chaser_State)
	{
	case ESCAPE:
		//cout << "ESCAPE" << endl;
		break;
	case PATROL:
		//cout << "PATROL" << endl;
		break;
	case CHASE:
		//cout << "CHASE" << endl;
		break;
	case ATTACK:
		//cout << "ATTACK" << endl;
		break;
	}
}

void CChaser::Render(float& _renderOrder)
{
	GraphicsManager::GetInstance()->GetModelStack().PushMatrix();
	GraphicsManager::GetInstance()->GetModelStack().Translate(
		this->position.x,
        this->position.y + _renderOrder,
		this->position.z);
	GraphicsManager::GetInstance()->GetModelStack().Scale(
		this->scale.x,
		this->scale.y,
		this->scale.z);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));
	GraphicsManager::GetInstance()->GetModelStack().PopMatrix();
}