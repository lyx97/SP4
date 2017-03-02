#pragma once

#include "SingletonTemplate.h"
#include <list>;
#include <queue>
#include "Particle/Particle.h"

static int NUMPARTICLESPAWN = 20;
static int MAXPARTICLE = 600;

class BatchRendering : public Singleton<BatchRendering>
{
public:
    void Update(double _dt);
    void Render();

    void AddParticleActive(void);
    void AddParticleInactive(void);
    void RemoveParticleActive(Particle* _Particle);
    void GetParticle(
        const Vector3& _pos,
        const Vector3& _scale,
        const Vector3& _color,
        const Vector3& _velocity,
        const int& _num,
        bool _distort = true);

    static std::list<Particle*> particleList;

private:
    friend Singleton<BatchRendering>;

    BatchRendering();
    virtual ~BatchRendering();

    void Splice(std::list<Particle*>::iterator _particle, bool _IsDone);

    std::list<Particle*> particleList_Active;
    std::list<Particle*> particleList_Inactive;

    std::queue<std::list<Particle*>::iterator> SpliceQueue;

    std::list<Vector3> floortileList;
    std::list<Vector3> walltileList;

    int prevRoomID;
};