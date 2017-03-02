#pragma once
#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Camera/Camera.h"
#include "../SpriteEntity.h"
#include "../TextEntity.h"

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
	float m_orthoWidth, m_orthoHeight;
	SpriteEntity* sprites[6];
	TextEntity* text[4];
	double MousePosX, MousePosY;
};
