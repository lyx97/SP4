#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Items/Powerup.h"

static const float ENEMY_MAX_SPEED = 50.0f;
static const float CHANCE_OF_DROPPING_POWERUP = 30.0f;

class Enemy2D : public GenericEntity
{
public:
	Enemy2D();
	virtual ~Enemy2D();

	virtual void Update(double _dt);
    virtual void Render(float& _renderOrder);

	Vector3 Cohesion(Enemy2D* enemy);
	Vector3 Separation(Enemy2D* enemy);
	Vector3 Alignment(Enemy2D* enemy);

	inline unsigned int GetDamage(){ return this->damage; };
	inline void SetDamage(unsigned int damage){ this->damage = damage; };

protected:
	float speed;
	float healthScale;
	float healthRatio;
	unsigned int damage;
	int prevHealth;
	bool nightmare;
};
