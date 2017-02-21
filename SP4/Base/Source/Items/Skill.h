#pragma once
#include "../GenericEntity.h"

class Skill : public GenericEntity
{
public:
	enum SKILL
	{
		INVINCIBLE,

		NUM_SKILL
	}skill;

	Skill();
	virtual ~Skill();

	void Init();
	virtual void Update(double _dt);
	virtual void Render();
};

