#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "../Camera/Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/SceneNode.h"
#include "Waypoint/WaypointManager.h"
#include "../Lua/LuaInterface.h"
#include "Enemy/Enemy.h"
#include "Enemy/Enemy2D.h"
#include "../Items/Powerup.h"
#include "Enemy/Chaser.h"
#include "Level/Minimap.h"

class SceneManager;
class TextEntity;
class Light;
class Color;

class SceneText : public Scene
{
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	CPlayerInfo* playerInfo;

    Camera camera;
	float m_worldWidth, m_worldHeight;
	float m_orthoWidth, m_orthoHeight;
	Vector3 mousePos_screenBased;
	Vector3 mousePos_worldBased;

	TextEntity* textObj[3];
	Light* lights[2];

	GenericEntity* theCube;
	CEnemy* theEnemy;
	Enemy2D* enemy2D;
	CChaser* chaser;


	bool GamePaused = false;
	
    CMinimap* minimap;
};

#endif