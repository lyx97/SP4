#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"

static const float ENEMY_MAX_SPEED = 50.0f;

class Enemy2D : public GenericEntity
{
public:
	Enemy2D();
	virtual ~Enemy2D();

	void Init();
	virtual void Update(double _dt);
	virtual void Render();

	Vector3 Cohesion(Enemy2D* enemy);
	Vector3 Separation(Enemy2D* enemy);
	Vector3 Alignment(Enemy2D* enemy);

protected:
	float speed;
};
