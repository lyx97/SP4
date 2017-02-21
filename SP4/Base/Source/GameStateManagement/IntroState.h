#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Camera/Camera.h"
#include "../SpriteEntity.h"

class SceneManager;
//class TextEntity;

class CIntroState : public Scene
{
public:
	CIntroState();
	~CIntroState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	SpriteEntity* IntroStateBackground;
	double MousePosX, MousePosY;
	// TextEntity* textObj[3];
};