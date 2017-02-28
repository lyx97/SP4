#pragma once
#include "Enemy2D.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Waypoint/WaypointManager.h"
#include "../Lua/LuaInterface.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

class CChaser :
	public GenericEntity
{
public:
	enum STATE
	{
		PATROL,
		CHASE,
		ATTACK,
		ESCAPE
	};

	CChaser();
	~CChaser();

	void Update(double dt);
    void Render(float& _renderOrder);
	void RanPosition();
	void Respond();
	void FSMUpdate();
	void RunFSM();
	void FindEscapeRoute();

	void MovePos(Vector3 finalpos, float speed);
	bool ReachPos(Vector3 finalpos);

private:
	CPlayerInfo* playerInfo;
	CWaypointManager* ChaserWaypoint;
	CWeaponInfo* ChaserBlast;
	int Chaser_State;
	int hp;
	int goToPosition;
	int ChaserPoint1, ChaserPoint2, ChaserPoint3;
	Vector3 newChaserPos;
	Vector3 EscapePos;
	float speed;
	float counter;
};
