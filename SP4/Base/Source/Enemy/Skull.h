#pragma once

#include "../GenericEntity.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "../WeaponInfo/LaserBlaster.h"

class CLevel;
class CPlayerInfo;

class Skull : public GenericEntity
{
private:
    enum FSM
    {
        MOVE,
        CHASE,
        ATTACK,
        DEAD,

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

    FSM fsm;

    CWeaponInfo* secondaryWeapon;

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