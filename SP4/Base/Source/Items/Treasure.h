#pragma once
#include <string>

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
		DOUBLE_DAMAGE,
		INVINCIBLE,
		INCREASE_FIRERATE,

		NUM_TREASURE
	}treasure_type;

    Treasure(); // for player
    Treasure(const int& _roomID); // for spawning
	virtual ~Treasure();

    void SpawnTreasure(Vector3 pos, unsigned int type, const int& _roomID);
	void Effects();
	virtual void Render(float& _renderOrder);
	void SetValues();

	inline int GetCooldown() { return cooldown; };
	inline float GetDuration() { return duration; };
	inline std::string GetName() { return name; };
	unsigned int random;

private:
	float duration;
	int cooldown;
	std::string name;
};
