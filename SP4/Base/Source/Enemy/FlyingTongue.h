#pragma once

#include "../GenericEntity.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "../WeaponInfo/LaserBlaster.h"

class CLevel;
class CPlayerInfo;

class FlyingTongue : public GenericEntity
{
private:
    enum FSM
    {
        MOVE,
        ATTACK,
        DEAD,

        NUM_FSM
    };

    double m_dSpeed;
    double m_dAcceleration;
    double m_dResponseTime;

    const double m_dResponse = 1.0;

    bool m_bAttackAnimation;
    bool m_bAlive;

    SpriteAnimation* currentAnimation;

    SpriteAnimation* moveLeft;
    SpriteAnimation* moveRight;
    SpriteAnimation* attackLeft;
    SpriteAnimation* attackRight;
    SpriteAnimation* dieLeft;
    SpriteAnimation* dieRight;

    FSM fsm;

public:
    FlyingTongue(const int _roomID);
    virtual ~FlyingTongue(void);

    // Update
    virtual void Update(double dt = 0.0333f);
    // Render
    virtual void Render(void);

    // Constrain the position within the borders
    void Constrain(void);

    inline bool GetAlive(void) { return m_bAlive; }
    inline void SetAlive(bool alive) { m_bAlive = alive; }
};