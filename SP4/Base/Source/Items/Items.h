#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

class Items : public GenericEntity
{
public:
	enum POWERUP
	{
		HEALTH_RECOVER,
		HEALTH_INCREASE,
		SPEED_INCREASE,


		NUM_POWERUP
	};

	Items();
	virtual ~Items();

	void Init();
	virtual void Update(double _dt);
	virtual void Render();

};

