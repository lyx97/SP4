#pragma once

#include "../SpriteAnimation/SpriteAnimation.h"
#include "Enemy2D.h"

class CLevel;
class CPlayerInfo;

class Spitter : public Enemy2D
{
private:
    enum FSM
    {
        IDLE,
        ATTACK,
        DEAD,

        NUM_FSM
    };

    Vector3 targetPos;

    double m_dSpeed;
    double m_dAcceleration;
    double m_dResponseTime;

    const double m_dResponse = 1.0;

    bool m_bAttackAnimation;
    bool m_bAlive;

    SpriteAnimation* currentAnimation;

    SpriteAnimation* idleLeft;
    SpriteAnimation* idleRight;
    SpriteAnimation* attackLeft;
    SpriteAnimation* attackRight;
    SpriteAnimation* dieLeft;
    SpriteAnimation* dieRight;

    FSM fsm;

public:
    Spitter(const int _roomID);
    virtual ~Spitter(void);

    // Update
    virtual void Update(double dt = 0.0333f);
    // Render
    virtual void Render(float& _renderOrder);

    // Constrain the position within the borders
    void Constrain(void);

    inline bool GetAlive(void) { return m_bAlive; }
    inline void SetAlive(bool alive) { m_bAlive = alive; }
};