#include "Minimap.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Application.h"
#include "../EntityManager.h"

CMinimap::CMinimap()
    : position(Vector3(0, 0, 0))
    , scale(Vector3(0, 0, 0))
{
    Border = MeshBuilder::GetInstance()->GetMesh("wall");
    Background = MeshBuilder::GetInstance()->GetMesh("wall");
    Rooms = MeshBuilder::GetInstance()->GetMesh("wall");
    Player = MeshBuilder::GetInstance()->GetMesh("wall");
}

CMinimap::~CMinimap()
{

}

void CMinimap::Init(void)
{
    position.Set(Vector3((Application::GetInstance().GetWindowWidth()), (Application::GetInstance().GetWindowHeight() >> 1), 10));
    scale.Set(150, 150, 1);

    EntityManager::GetInstance()->AddEntity(this);
}

void CMinimap::Update(double dt)
{
    position.Set(Vector3((Application::GetInstance().GetWindowWidth() >> 1) - 100, (Application::GetInstance().GetWindowHeight() >> 1) - 100, 10));
}

void CMinimap::RenderUI(void)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    //modelStack.Rotate(-90, 1, 0, 0);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GRIDMESH"));
    modelStack.PopMatrix();
}
