#pragma once

#include <vector>
#include "CameraBase.h"
#include "../GenericEntity.h"

class CPlayerInfo;

class Camera : public CameraBase
{
public:
    Camera();
    virtual ~Camera();

    virtual Mtx44 GetViewMatrix();

    virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    virtual void Update(double dt);
    virtual void Reset();

    virtual Vector3 GetCameraPos() const;
    virtual void SetCameraPos(Vector3 pos);
    virtual Vector3 GetCameraTarget() const;
    virtual void SetCameraTarget(Vector3 pos);
    virtual Vector3 GetCameraUp() const;
    virtual void SetCameraUp(Vector3 pos);

	void Constrain(EntityBase* constrain, int radius);

	std::vector<Vector3> entityList;

private:
    Vector3 tempTarget;
    float m_worldWidth, m_worldHeight;

    Vector3 position;
    Vector3 target;
    Vector3 up;

    Vector3 defaultPosition;
    Vector3 defaultTarget;
    Vector3 defaultUp;
};