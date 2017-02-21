#pragma once

#include <list>
#include "../GenericEntity.h"

class Particle : public GenericEntity 
{
public:
    enum PARTICLE_EFFECT
    {
        GRAVITY,
        SHRINK,
        EXPAND,
        NUM_PARTICLEEFFECT
    }effect;

    Particle();
    ~Particle();

    void Init(void);

    inline Vector3 GetVelocity(void) const { return velocity; }
    inline void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }

    inline float GetRotation(void) const { return m_fRotation; }
    inline void SetRotation(const float& rotation) { this->m_fRotation = rotation; }

    inline float GetRotationSpeed(void) const { return m_fRotationSpeed; }
    inline void SetRotationSpeed(const float& rotationspeed) { this->m_fRotationSpeed = rotationspeed; }

    inline double GetLifetime(void) const { return m_dLifetime; }
    inline void SetLifetime(const double& lifetime) { this->m_dLifetime = lifetime; }

    void AddEffect(PARTICLE_EFFECT effect);

    // Update
    void Update(double dt = 0.0333f);

    // Render
    void Render(void);

protected:
    Vector3 velocity;           // Velocity of Particle

    float m_fRotation;          // Rotation of Particle
    float m_fRotationSpeed;     // Rotational speed of Particle
    float m_fGravity;           // Gravity of particle

    float m_fShrinkSpeed;       // Speed of shrinking
    float m_fExpandSpeed;       // Speed of expanding

    float m_fMinSize;           // Min size of particle, default is EPSILON
    float m_fMaxSize;           // Max size of particle, default is 10

    double m_dLifetime;         // Lifetime of particle

    std::list<PARTICLE_EFFECT> effectList;
};