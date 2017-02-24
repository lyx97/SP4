#pragma once

#include "../GenericEntity.h"
#include "../EntityManager.h"
#include "../Application.h"
#include "MathUtility.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"

class Treasure : public GenericEntity
{
public:
	enum TREASURES
	{
		NONE,
		RAPID_HEALTHREGEN,
		SPRINT,
		ONE_HIT_KILL,
		INVINCIBLE,

		NUM_TREASURE
	}treasure;

	Treasure();
	virtual ~Treasure();

	void Init();
	void SpawnTreasure(Vector3 pos, TREASURES type);
	virtual void Update(double _dt);
	virtual void Render();
	void SetValues();
	void SwapTreasures();

	inline int GetCooldown() { return cooldown; };
	inline int GetDuration() { return duration; };

private:
	unsigned int random;
	int cooldown;
	float duration;
};
