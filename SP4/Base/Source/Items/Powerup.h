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
		DAMAGE_INCREASE,

		NUM_POWERUP
	}powerup;

    Powerup(const int& _roomID);
    Powerup(Vector3 pos, const int& _roomID);
	virtual ~Powerup();

	void Init();
	virtual void Update(double _dt);
	virtual void Render(float& _renderOrder);

private:
	unsigned int random;
};

