#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

class Treasure : public GenericEntity
{
public:
	enum TREASURES
	{
		NONE,
		DASH_COOLDOWN,
		HEALTH_INCREASE,
		SPEED_INCREASE,
		HEALTHREGEN_INCREASE,

		NUM_TREASURE
	}treasure;

	Treasure();
	Treasure(int type);
	virtual ~Treasure();

	void Init();
	virtual void Update(double _dt);
	virtual void Render();

	inline int GetCooldown() { return cooldown; };

private:
	unsigned int random;
	int cooldown;

};
