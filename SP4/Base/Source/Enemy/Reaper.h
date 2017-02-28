#pragma once

#include "../GenericEntity.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../WeaponInfo/WeaponInfo.h"
#include "../WeaponInfo/LaserBlaster.h"

class CLevel;
class CPlayerInfo;

class Reaper : public GenericEntity
{
private:
    enum FSM
    {
        MOVE,
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

    float HPScale;
    int prevHP;

    SpriteAnimation* currentAnimation;

    SpriteAnimation* moveLeft;
    SpriteAnimation* moveRight;
    SpriteAnimation* attackLeft;
    SpriteAnimation* attackRight;
    SpriteAnimation* dieLeft;
    SpriteAnimation* dieRight;

    SpriteAnimation* smoke;

    FSM fsm;

public:
    Reaper(const int _roomID);
    virtual ~Reaper(void);

    // Update
    virtual void Update(double dt = 0.0333f);
    // Render
    virtual void Render(float& _renderOrder);
    virtual void RenderUI(void);

    // Constrain the position within the borders
    void Constrain(void);

    inline bool GetAlive(void) { return m_bAlive; }
    inline void SetAlive(bool alive) { m_bAlive = alive; }
};