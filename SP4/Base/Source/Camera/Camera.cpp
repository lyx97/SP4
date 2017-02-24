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
    //Vector3 view = (target - position).Normalized();
    //Vector3 right = view.Cross(up);
    //right.y = 0;
    //right.Normalize();
    //this->up = defaultUp = right.Cross(view).Normalized();
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
	//this->target.Lerp(temp, 2 * dt);
	this->position = this->target;
	this->position.z += 1; // the thing making the camera 45 degrees
	this->position.y += 1;

    //double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
    //double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

    //if (KeyboardController::GetInstance()->IsKeyDown('J'))
    //{
    //    Vector3 view = (target - position).Normalized();
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    position -= right * CAMERA_SPEED * (float)dt;
    //    target -= right * CAMERA_SPEED * (float)dt;
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown('L'))
    //{
    //    Vector3 view = (target - position).Normalized();
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    position += right * CAMERA_SPEED * (float)dt;
    //    target += right * CAMERA_SPEED * (float)dt;
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown('I'))
    //{
    //    Vector3 view = (target - position).Normalized();
    //    position += view * CAMERA_SPEED * (float)dt;
    //    target += view * CAMERA_SPEED * (float)dt;
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown('K'))
    //{
    //    Vector3 view = (target - position).Normalized();
    //    position -= view * CAMERA_SPEED * (float)dt;
    //    target -= view * CAMERA_SPEED * (float)dt;
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
    //{
    //    Vector3 view = (target - position).Normalized();
    //    float yaw = (float)(CAMERA_SPEED * (float)dt);
    //    Mtx44 rotation;
    //    rotation.SetToRotation(yaw, 0, 1, 0);
    //    view = rotation * view;
    //    target = position + view;
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    up = right.Cross(view).Normalized();
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
    //{
    //    Vector3 view = (target - position).Normalized();
    //    float yaw = (float)(-CAMERA_SPEED * (float)dt);
    //    Mtx44 rotation;
    //    rotation.SetToRotation(yaw, 0, 1, 0);
    //    view = rotation * view;
    //    target = position + view;
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    up = right.Cross(view).Normalized();
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
    //{
    //    float pitch = (float)(CAMERA_SPEED * (float)dt);
    //    Vector3 view = (target - position).Normalized();
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    up = right.Cross(view).Normalized();
    //    Mtx44 rotation;
    //    rotation.SetToRotation(pitch, right.x, right.y, right.z);
    //    view = rotation * view;
    //    target = position + view;
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
    //{
    //    float pitch = (float)(-CAMERA_SPEED * (float)dt);
    //    Vector3 view = (target - position).Normalized();
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    up = right.Cross(view).Normalized();
    //    Mtx44 rotation;
    //    rotation.SetToRotation(pitch, right.x, right.y, right.z);
    //    view = rotation * view;
    //    target = position + view;
    //}
    //Update the camera direction based on mouse move
    // left-right rotate
    //{
    //    Vector3 view = (target - position).Normalized();
    //    float yaw = (float)(-CAMERA_SPEED * camera_yaw * (float)dt);
    //    Mtx44 rotation;
    //    rotation.SetToRotation(yaw, 0, 1, 0);
    //    view = rotation * view;
    //    target = position + view;
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    up = right.Cross(view).Normalized();
    //}
    //{
    //    float pitch = (float)(-CAMERA_SPEED * camera_pitch * (float)dt);
    //    Vector3 view = (target - position).Normalized();
    //    Vector3 right = view.Cross(up);
    //    right.y = 0;
    //    right.Normalize();
    //    up = right.Cross(view).Normalized();
    //    Mtx44 rotation;
    //    rotation.SetToRotation(pitch, right.x, right.y, right.z);
    //    view = rotation * view;
    //    target = position + view;
    //}
    //if (KeyboardController::GetInstance()->IsKeyDown('R'))
    //{
    //    Reset();
    //}
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