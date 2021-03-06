#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Camera/Camera.h"
#include "../SpriteEntity.h"
#include "../TextEntity.h"

class SceneManager;

class LoseState : public Scene
{
public:
	LoseState();
	~LoseState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera camera;
	float m_orthoWidth, m_orthoHeight;
	double MousePosX, MousePosY;
	SpriteEntity* sprite;
	TextEntity* text;
	TextEntity* text1;
};