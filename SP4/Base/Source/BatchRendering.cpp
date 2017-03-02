#include "BatchRendering.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "Level/Level.h"
#include "LoadTGA.h"

std::list<Particle*> BatchRendering::particleList;

BatchRendering::BatchRendering()
    : prevRoomID(-1)
{
}

BatchRendering::~BatchRendering()
{
}

void BatchRendering::Update(double _dt)
{
    if (floortileList.size() <= 0)
    {
        for (int x = 0; x < ROOMSIZE; ++x)
        {
            for (int z = 0; z < ROOMSIZE; ++z)
            {
                if (CLevel::GetInstance()->GetRoom(0)->GetSpatialPartition()->GetGridType(x, z) == GRID_TYPE::PATH ||
                    CLevel::GetInstance()->GetRoom(0)->GetSpatialPartition()->GetGridType(x, z) == GRID_TYPE::DOORPATH)
                    floortileList.push_back(CLevel::GetInstance()->GetRoom(0)->GetSpatialPartition()->GetGridPos(x, z));
            }
        }
        MeshBuilder::GetInstance()->GenerateTile("floortile", floortileList);
        MeshBuilder::GetInstance()->GetMesh("floortile")->textureID = LoadTGA("Image//Tile//floor.tga");
    }
    if (prevRoomID != CPlayerInfo::GetInstance()->GetRoomID())
    {
        walltileList.clear();

        for (int x = 0; x < ROOMSIZE; ++x)
        {
            for (int z = 0; z < ROOMSIZE; ++z)
            {
                if (CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetSpatialPartition()->GetGridType(x, z) == GRID_TYPE::WALL)
                    walltileList.push_back(CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetSpatialPartition()->GetGridPos(x, z));
            }
        }

        MeshBuilder::GetInstance()->GenerateTile("walltile", walltileList);
        MeshBuilder::GetInstance()->GetMesh("walltile")->textureID = LoadTGA("Image//Tile//wall2.tga");

        prevRoomID = CPlayerInfo::GetInstance()->GetRoomID();
    }

    // Update particles
    std::list<Particle*>::iterator it2, end2;
    end2 = particleList_Active.end();
    for (it2 = particleList_Active.begin(); it2 != end2; ++it2)
    {
        (*it2)->Update(_dt);
        if ((*it2)->IsDone())
            SpliceQueue.push(it2);
    }

    while (SpliceQueue.size() > 0)
    {
        std::list<Particle*>::iterator particle = SpliceQueue.front();
        // To active or inactive list
        Splice(particle, (*particle)->IsDone());
        SpliceQueue.pop();
    }
}

void BatchRendering::Render()
{
    // Render particles
    std::list<Particle*>::iterator it, end;
    end = particleList_Active.end();
    int count = 0;
    for (it = particleList_Active.begin(); it != end; ++it)
    {
        particleList.push_back(*it);
        count++;
        if (count >= MAXPARTICLE)
            break;
    }
    if (particleList_Active.size() > 0)
    {
        MeshBuilder::GetInstance()->GenerateParticle("particle");
        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("particle"));
        particleList.clear();
    }

    if (floortileList.size() > 0)
        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("floortile"));
    if (walltileList.size() > 0)
        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("walltile"));
}

// Add a particle to active list
void BatchRendering::AddParticleActive(void)
{
    Particle* particle = new Particle();
    particleList_Active.push_back(particle);
}

// Add a particle to inactive list
void BatchRendering::AddParticleInactive(void)
{
    Particle* particle = new Particle();
    particle->Init();
    particleList_Inactive.push_back(particle);
}

void BatchRendering::RemoveParticleActive(Particle* _Particle)
{
    std::list<Particle*>::iterator findIter = std::find(particleList_Active.begin(), particleList_Active.end(), _Particle);

    // Delete the entity if found
    if (findIter != particleList_Active.end())
    {
        delete *findIter;
        findIter = particleList_Active.erase(findIter);

        return;
    }
}

void BatchRendering::GetParticle(
    const Vector3& _pos,
    const Vector3& _scale,
    const Vector3& _color,
    const Vector3& _velocity,
    const int& _num,
    bool _distort)
{
    for (int i = 0; i < _num; ++i)
    {
        if (particleList_Inactive.size() > 0)
        {
            std::list<Particle*>::iterator it = particleList_Inactive.begin();
            if ((*it)->IsDone())
            {
                (*it)->SetIsDone(false);
                (*it)->SetPosition(_pos);
                (*it)->SetScale(_scale);
                (*it)->SetColor(_color);
                (*it)->SetLifetime(Math::RandFloatMinMax(0.3f, 1.6f));
                (*it)->SetVelocity(_velocity + Vector3(Math::RandFloatMinMax(-20.f, 20.f), 0, Math::RandFloatMinMax(-20.f, 20.f)));
                (*it)->SetDistort(_distort);
                SpliceQueue.push(it);
            }

            Particle* particle = new Particle();
            particle->SetPosition(_pos);
            particle->SetScale(_scale);
            particle->SetColor(_color);
            particle->SetLifetime(Math::RandFloatMinMax(0.3f, 1.6f));
            particle->SetVelocity(_velocity + Vector3(Math::RandFloatMinMax(-20.f, 20.f), 0, Math::RandFloatMinMax(-20.f, 20.f)));
            particle->SetDistort(_distort);
            particleList_Active.push_back(particle);
        }
        else
        {
            for (int i = 0; i < 10; ++i)
                AddParticleInactive();

            Particle* particle = new Particle();
            particle->SetPosition(_pos);
            particle->SetScale(_scale);
            particle->SetColor(_color);
            particle->SetLifetime(Math::RandFloatMinMax(0.3f, 1.6f));
            particle->SetVelocity(_velocity + Vector3(Math::RandFloatMinMax(-20.f, 20.f), 0, Math::RandFloatMinMax(-20.f, 20.f)));
            particle->SetDistort(_distort);
            particleList_Active.push_back(particle);
        }
    }
}

void BatchRendering::Splice(std::list<Particle*>::iterator _particle, bool _IsDone)
{
    if (_IsDone)
        particleList_Inactive.splice(particleList_Inactive.end(), particleList_Active, _particle);
    else
        particleList_Active.splice(particleList_Active.end(), particleList_Inactive, _particle);
}