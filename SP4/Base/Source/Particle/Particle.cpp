#include "Particle.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

Particle::Particle(void)
    : GenericEntity(NULL)
    , velocity(Vector3(0, 0, 0))
    , m_fRotation(0.f)
    , m_fRotationSpeed(0.f)
    , m_fGravity(9.8f)
    , m_fExpandSpeed(1.0f)
    , m_fShrinkSpeed(1.0f)
    , m_fMinSize(Math::EPSILON)
    , m_fMaxSize(10.f)
{

}

Particle::~Particle(void)
{

}

void Particle::Init(void)
{
    isDone = true;

    position = Vector3(0, 20, 0);
    scale = Vector3(1, 1, 1);

    m_dLifetime = 10.0;

    velocity.x = Math::RandFloatMinMax(-20.f, 20.f);
    velocity.z = Math::RandFloatMinMax(-20.f, 20.f);
}

void Particle::AddEffect(PARTICLE_EFFECT effect)
{
    effectList.push_back(effect);
}

// Update
void Particle::Update(double dt)
{
    position += velocity * dt;

    std::list<PARTICLE_EFFECT>::iterator it, end;
    end = effectList.end();
    for (it = effectList.begin(); it != end; ++it)
    {
        if ((*it) == Particle::GRAVITY)
        {
            position.y -= m_fGravity * dt;

            //if (position.y < -10.f)
            //    SetIsDone(true);
        }
        if ((*it) == Particle::SHRINK)
        {
            scale.x -= dt * m_fShrinkSpeed;
            scale.y -= dt * m_fShrinkSpeed;
            scale.z -= dt * m_fShrinkSpeed;

            if (scale.x < m_fMinSize ||
                scale.y < m_fMinSize ||
                scale.z < m_fMinSize)
                SetIsDone(true);
        }
        if ((*it) == Particle::EXPAND)
        {
            scale.x += dt * m_fExpandSpeed;
            scale.y += dt * m_fExpandSpeed;
            scale.z += dt * m_fExpandSpeed;

            if (scale.x > m_fMaxSize ||
                scale.y > m_fMaxSize ||
                scale.z > m_fMaxSize)
                SetIsDone(true);
        }
    }


    m_dLifetime -= dt;
    if (m_dLifetime <= 0.0)
        SetIsDone(true);
}

// Render
void Particle::Render(void)
{
    //MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    //modelStack.PushMatrix();
    //modelStack.Translate(position.x, position.y, position.z);
    ////modelStack.Scale(scale.x, scale.y, scale.z);
    //RenderHelper::StoreParticlesMVP();
    ////RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GRIDMESH"));
    //modelStack.PopMatrix();
}