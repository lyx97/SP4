#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

class Powerup : public GenericEntity
{
public:
	enum POWERUP
	{
		HEALTH_RECOVER,
		HEALTH_INCREASE,
		SPEED_INCREASE,
		HEALTHREGEN_INCREASE,

		NUM_POWERUP
	}powerup;



	Powerup();	
	Powerup(Vector3 pos);
	virtual ~Powerup();

	void Init();
	virtual void Update(double _dt);
	virtual void Render();

private:
	unsigned int random;
};

