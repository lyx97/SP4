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
	enum TREASURES_TYPE
	{
		NONE,
		RAPID_HEALTHREGEN,
		SPRINT,
		ONE_HIT_KILL,
		INVINCIBLE,

		NUM_TREASURE
	}treasure_type;

	Treasure();
	virtual ~Treasure();

	void Init();
	void SpawnTreasure(Vector3 pos, unsigned int type);
	void Effects();
	virtual void Render();
	void SetValues();

	inline int GetCooldown() { return cooldown; };
	inline float GetDuration() { return duration; };
	unsigned int random;

private:
	float duration;
	int cooldown;

};
