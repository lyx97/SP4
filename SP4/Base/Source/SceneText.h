#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "../Camera/Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "../Lua/LuaInterface.h"
#include "Enemy/Enemy2D.h"
#include "../Items/Powerup.h"
#include "../Items/Treasure.h"
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

    void UpdatePause();
    void RenderPause();

private:
    enum PAUSESTATE
    {
        RESUME,
        MENU,
        QUIT,
        NUM_PAUSESTATE
    }state;

	CPlayerInfo* playerInfo;

    Camera camera;
	float m_worldWidth, m_worldHeight;
	float m_orthoWidth, m_orthoHeight;
	Vector3 mousePos_screenBased;
	Vector3 mousePos_worldBased;

	TextEntity* textObj[3];
	Light* lights[2];

	bool GamePaused = false;

    float buttonScale[3];
	
    CMinimap* minimap;
};

#endif