#pragma once
#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Camera/Camera.h"
#include "../SpriteEntity.h"

class SceneManager;

class CHelpState :
	public Scene
{
public:
	CHelpState();
	~CHelpState();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	SpriteEntity* MenuStateBackground;
	double MousePosX, MousePosY;
};
