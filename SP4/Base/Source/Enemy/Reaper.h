#pragma once

#include "../SpriteAnimation/SpriteAnimation.h"
#include "Enemy2D.h"
#include "Smoke.h"

class CLevel;
class CPlayerInfo;

class Reaper : public Enemy2D
{
private:
    enum FSM
    {
        MOVE,
        SMOKEATTACK,
        SCYTHEATTACK,
        DEAD,
        NUM_FSM
    };

    Vector3 targetPos;

    double m_dSpeed;
    double m_dAcceleration;
    double m_dResponseTime;

    const double m_dResponse = 2.0;

    bool m_bAttackAnimation;
    bool m_bAlive;
    bool m_bInvulnerable;

    float HPScale;
    int prevHP;

    SpriteAnimation* currentAnimation;
    
    SpriteAnimation* attackLeft;
    SpriteAnimation* attackRight;
    SpriteAnimation* dieLeft;
    SpriteAnimation* dieRight;

    SpriteAnimation* moveLeft2;
    SpriteAnimation* moveRight2;
    SpriteAnimation* attackLeft2;
    SpriteAnimation* attackRight2;
    SpriteAnimation* dieLeft2;
    SpriteAnimation* dieRight2;
    
    Smoke* smoke[4];
    bool m_bSmokeActive[4];

    FSM fsm;

public:
    Reaper(const int _roomID);
    virtual ~Reaper(void);

    // Update
    virtual void Update(double dt = 0.0333f);
    // Render
    virtual void Render(float& _renderOrder);

    // Constrain the position within the borders
    void Constrain(void);

    inline bool GetAlive(void) { return m_bAlive; }
    inline void SetAlive(bool alive) { m_bAlive = alive; }
};