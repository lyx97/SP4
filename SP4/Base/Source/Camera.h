#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Player.h"
#include "GenericEntity.h"
#include <vector>

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	bool isDeadzoned;
	Vector3 tempTarget;
	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

	float m_worldWidth, m_worldHeight;

	void Include(Vector3* pos);

	bool Deadzone(Vector3 *p, Vector3 cameraTarget, float ortho);
	void Constrain(GenericEntity* constrain, float radius);

	Vector3 GetPosition();
	void Constrain(Player p, Vector3 cameraTarget);

	//Vector List of entities taken into account for camera position
	std::vector<Vector3*>entityList;
};

#endif