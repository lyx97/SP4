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

    inline float GetRotation(void) const { return m_fRotation; }
    inline void SetRotation(const float& rotation) { this->m_fRotation = rotation; }

    inline float GetRotationSpeed(void) const { return m_fRotationSpeed; }
    inline void SetRotationSpeed(const float& rotationspeed) { this->m_fRotationSpeed = rotationspeed; }

    inline double GetLifetime(void) const { return m_dLifetime; }
    inline void SetLifetime(const double& lifetime) { this->m_dLifetime = lifetime; }

    inline void SetDistort(bool _distort) { m_bDistort = _distort; }
    inline bool GetDistort(void) { return m_bDistort; }

    void AddEffect(PARTICLE_EFFECT effect);

    // Update
    void Update(double dt = 0.0333f);

protected:

    float m_fRotation;          // Rotation of Particle
    float m_fRotationSpeed;     // Rotational speed of Particle
    float m_fGravity;           // Gravity of particle

    float m_fShrinkSpeed;       // Speed of shrinking
    float m_fExpandSpeed;       // Speed of expanding

    float m_fMinSize;           // Min size of particle, default is EPSILON
    float m_fMaxSize;           // Max size of particle, default is 10

    double m_dLifetime;         // Lifetime of particle

    bool m_bDistort;

    std::list<PARTICLE_EFFECT> effectList;
};