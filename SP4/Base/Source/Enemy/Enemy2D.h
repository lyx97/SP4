#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"

class Enemy2D : public GenericEntity
{
public:
	Enemy2D();
	virtual ~Enemy2D();

	void Init();
	virtual void Update(double _dt);
	virtual void Render();
	void GoToRandomPos(Vector3 pos);

	Vector3 randomPos;
	Vector3 leaderPos;
	bool isLeader;
	int neighbour = 0;

protected:
	float speed;
};

