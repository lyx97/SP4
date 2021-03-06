#include "Minimap.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../Application.h"
#include "../EntityManager.h"
#include "../Level/Level.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "LoadTGA.h"

CMinimap::CMinimap()
    : position(Vector3(0, 0, 0))
{
    Border = MeshBuilder::GetInstance()->GenerateQuad("minimapborder", Color(0.f, 0.f, 0.f), 1.f);
    Background = MeshBuilder::GetInstance()->GenerateQuad("minimapbackground", Color(0.f, 0.f, 0.f), 0.9f);
    MeshBuilder::GetInstance()->GetMesh("minimapbackground")->textureID = LoadTGA("Image//Tile//floor.tga");
    Room = MeshBuilder::GetInstance()->GetMesh("room");
    Player = MeshBuilder::GetInstance()->GenerateQuad("minimapplayer", Color(1.f, 1.f, 0.f), 1.f);

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
	position.Set(Vector3((Application::GetInstance().GetWindowWidth() >> 1) - 100, (Application::GetInstance().GetWindowHeight() >> 1) - 100, 0));

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	if (CPlayerInfo::GetInstance()->IsOnScreenUI())
	{
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
				0);
			modelStack.Scale(ROOMSCALE, ROOMSCALE, 1);
			RenderHelper::RenderMesh(Room);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z + 1);
		modelStack.Scale(PLAYERSCALE, PLAYERSCALE, 1);
		RenderHelper::RenderMesh(Player);
		modelStack.PopMatrix();

		glDisable(GL_STENCIL_TEST);
	}
}
