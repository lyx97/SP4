#pragma once
#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Camera/Camera.h"
#include "../SpriteEntity.h"

class SceneManager;

class COptionState :
	public Scene
{
public:
	COptionState();
	~COptionState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	SpriteEntity* MenuStateBackground;
	double MousePosX, MousePosY;
};
