#pragma once

#include "../SpriteAnimation/SpriteAnimation.h"
#include "Enemy2D.h"

class Smoke : public Enemy2D
{
private:
    SpriteAnimation* smoke;

    bool m_bSmokeActive;

public:
    Smoke(const Vector3 _pos, const int _roomID);
    virtual ~Smoke(void);

    // Update
    virtual void Update(double dt = 0.0333f);
    // Render
    virtual void Render(float& _renderOrder);

    inline bool GetActive(void) { return m_bSmokeActive; }
};