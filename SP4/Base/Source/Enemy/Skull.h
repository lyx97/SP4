#pragma once

#include "../SpriteAnimation/SpriteAnimation.h"
#include "Enemy2D.h"

class CLevel;
class CPlayerInfo;

class Skull : public Enemy2D
{
private:
    enum FSM
    {
        MOVE,
        CHASE,
        ATTACK,
        DEAD,
		TRANSFORMING,
		UNTRANSFORMING,
		TRANSFORMED,

        NUM_FSM
    };

    double m_dSpeed;
    double m_dAcceleration;
    double m_dResponseTime;

    const double m_dResponse = 0.5;

    bool m_bAttackAnimation;
    bool m_bAlive;

    SpriteAnimation* currentAnimation;

    SpriteAnimation* moveLeft;
    SpriteAnimation* moveRight;
    SpriteAnimation* attackLeft;
    SpriteAnimation* attackRight;
    SpriteAnimation* dieLeft;
    SpriteAnimation* dieRight;
	SpriteAnimation* transformingLeft;
	SpriteAnimation* transformingRight;
	SpriteAnimation* untransformingLeft;
	SpriteAnimation* untransformingRight;
	SpriteAnimation* transformedLeft;
	SpriteAnimation* transformedRight;

    FSM fsm;
	bool transformed;

public:
    Skull(const int _roomID);
    virtual ~Skull(void);

    // Update
    virtual void Update(double dt = 0.0333f);
    // Render
    virtual void Render(float& _renderOrder);

    // Constrain the position within the borders
    void Constrain(void);

    inline bool GetAlive(void) { return m_bAlive; }
    inline void SetAlive(bool alive) { m_bAlive = alive; }
};