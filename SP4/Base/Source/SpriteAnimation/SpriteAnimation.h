#pragma once
#include "Mesh.h"

struct Animation
{
	Animation() {}

	void Set(int startFrame, int endFrame, int repeat, float time, bool active)
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
		this->animActive = active;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
	bool animActive;
};

class SpriteAnimation : public Mesh
{
public:
    SpriteAnimation();
	SpriteAnimation(const std::string& meshName);
	~SpriteAnimation();
	void Update(double dt);
    virtual void Render();

	inline float GetTimeCurrent(){ return m_timeCurrent; };
	inline void SetTimeCurrent(float timeCurrent){ m_timeCurrent = timeCurrent; };
	inline int GetCurrentFrame(){ return m_currentFrame; };
	inline void SetCurrentFrame(float currentFrame){ m_currentFrame = currentFrame; };
	inline int GetPlayCount(){ return m_playCount; };
	inline void SetPlayCount(float playCount){ m_playCount = playCount; };

    inline void SetAttackFrame(int attackframe){ m_attackFrame = attackframe; };
    inline bool GetAttack(){ return m_attack; };

	Animation *m_anim;

private:
    int m_row;
    int m_col;

    float m_timeCurrent;
    int m_currentFrame;
    int m_playCount;
    int m_attackFrame;

    bool m_attack;
};