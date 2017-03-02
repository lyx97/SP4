#include "Obstacle.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "../EntityManager.h"
#include "../Level/Level.h"

Obstacle::Obstacle(const Vector3 _pos, const int _roomID)
    : GenericEntity(NULL)
{
    this->position = _pos;
    this->scale = Vector3(GRIDSIZE - 2, GRIDSIZE - 2, 0);
    this->velocity = Vector3(0, 0, 0);

    this->isDone = false;
    this->m_bCollider = true;
    this->m_bLaser = false;

    this->m_eEntityType = EntityBase::OBSTACLE;

    roomID = _roomID;

    this->SetCollider(true);
    int x = scale.x, y = scale.y;
    x = (x >> 1); y = (y >> 1);
    this->SetAABB(Vector3(x, y, 0), Vector3(-x, -y, 0));

    EntityManager::GetInstance()->AddEntity(this, roomID);

    // Particle
    color.Set(Vector3(0.f, 0.8f, 1.f));
}

Obstacle::~Obstacle()
{

}

void Obstacle::Update(double _dt)
{

}

void Obstacle::Render(float& _renderOrder)
{
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y - 0.01f, position.z);
    modelStack.Rotate(90, -1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("obstacle"));
    modelStack.PopMatrix();

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}