#include "Camera.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Application.h"
#include "../EntityManager.h"
#include "../PlayerInfo/PlayerInfo.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

Mtx44 Camera::GetViewMatrix()
{
    Mtx44 result;
    result.SetToLookAt(position.x, position.y, position.z,
        target.x, target.y, target.z,
        up.x, up.y, up.z);
    return result;
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->position = defaultPosition = pos;
    this->target = defaultTarget = target;
	this->up = defaultUp = up;
	m_worldHeight = 100.0f;
	m_worldWidth = m_worldHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
}

void Camera::Update(double dt)
{
    static const float CAMERA_SPEED = 200.f;

	Vector3 temp;
	temp = Application::GetInstance().GetWorldBasedMousePos() + CPlayerInfo::GetInstance()->GetPosition() + this->target;
	temp.x = temp.x * 0.5f;
	temp.z = temp.z * 0.5f;
	this->target.Lerp(temp, dt);
	this->position = this->target;
	this->position.z += 1; // the thing making the camera 45 degrees
	this->position.y += 1;
}

void Camera::Constrain(EntityBase* constrain, int radius)
{
	if (this->target.x > constrain->GetPosition().x + radius)
	{
		this->target.x = constrain->GetPosition().x + radius;
		this->position.x = constrain->GetPosition().x + radius;
	}
	else if (this->target.x < constrain->GetPosition().x - radius)
	{
		this->target.x = constrain->GetPosition().x - radius;
		this->position.x = constrain->GetPosition().x - radius;
	}

	if (this->target.z > constrain->GetPosition().z + radius)
	{
		this->target.z = constrain->GetPosition().z + radius;
		this->position.z = constrain->GetPosition().z + radius;
		this->position.z += 1;
	}
	else if (this->target.z < constrain->GetPosition().z - radius)
	{
		this->target.z = constrain->GetPosition().z - radius;
		this->position.z = constrain->GetPosition().z - radius;
		this->position.z += 1;
	}
}

Vector3 Camera::GetCameraPos() const
{
    return position;
}

void Camera::SetCameraPos(Vector3 pos)
{
    position = pos;
}

Vector3 Camera::GetCameraTarget() const
{
    return target;
}

void Camera::SetCameraTarget(Vector3 target)
{
    this->target = target;
}

Vector3 Camera::GetCameraUp() const
{
    return up;
}

void Camera::SetCameraUp(Vector3 up)
{
    this->up = up;
}

void Camera::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}