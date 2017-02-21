#include "Minimap.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Application.h"
#include "../EntityManager.h"
#include "../Level/Level.h"
#include "../PlayerInfo/PlayerInfo.h"

CMinimap::CMinimap()
    : position(Vector3(0, 0, 0))
{
    Border = MeshBuilder::GetInstance()->GetMesh("wall");
    Background = MeshBuilder::GetInstance()->GetMesh("GRIDMESH");
    Path = MeshBuilder::GetInstance()->GetMesh("room");
    Room = MeshBuilder::GetInstance()->GetMesh("room");
    Player = MeshBuilder::GetInstance()->GetMesh("cube");

    position.Set(Vector3((Application::GetInstance().GetWindowWidth() >> 1) - 100, (Application::GetInstance().GetWindowHeight() >> 1) - 100, 0));

    EntityManager::GetInstance()->AddEntity(this);
}

CMinimap::~CMinimap()
{

}

void CMinimap::Update(double dt)
{
    position.Set(Vector3((Application::GetInstance().GetWindowWidth() >> 1) - 100, (Application::GetInstance().GetWindowHeight() >> 1) - 100, 0));



}

void CMinimap::RenderUI(void)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(BGSCALE, BGSCALE, 1);
    RenderHelper::RenderMesh(Border);
    RenderHelper::RenderMesh(Background);
    modelStack.PopMatrix();

    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    glDepthMask(GL_TRUE); // Write to depth buffer

    for (int i = 0; i < CLevel::GetInstance()->GetNumRoom(); ++i)
    {
        modelStack.PushMatrix();
        modelStack.Translate(position.x, position.y, position.z);
        modelStack.Translate
            ((CLevel::GetInstance()->GetRoomIndex()[i].x * 25) - (CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetIndex().x * 25),
            -((CLevel::GetInstance()->GetRoomIndex()[i].z * 25) - (CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetIndex().z * 25)),
            1);
        modelStack.Scale(ROOMSCALE, ROOMSCALE, 1);
        RenderHelper::RenderMesh(Room);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z + 2);
    modelStack.Scale(PLAYERSCALE, PLAYERSCALE, 1);
    RenderHelper::RenderMesh(Player);
    modelStack.PopMatrix();

    glDisable(GL_STENCIL_TEST);
}
