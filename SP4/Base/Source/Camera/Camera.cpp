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
    //Vector3 view = (target - position).Normalized();
    //Vector3 right = view.Cross(up);
    //right.y = 0;
    //right.Normalize();
    //this->up = defaultUp = right.Cross(view).Normalized();
    this->up = defaultUp = up;

	m_worldHeight = 100.0f;
	m_worldWidth = m_worldHeight * (float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight();
	isDeadzoned = false;
}

void Camera::Update(double dt)
{
    static const float CAMERA_SPEED = 200.f;


	Vector3 temp;
	//if (!entityList.empty())
	//{
	//	for (int i = 0; i < entityList.size(); ++i)
	//	{
	//		temp += entityList[i];
	//	}
	//	temp / entityList.size();

	//	this->target.Lerp(temp, 5 * dt);
	//	this->position = this->target;
	//	this->position.z += 1;
	//	this->position.y += 1;
	//}

	temp = Application::GetInstance().GetWorldBasedMousePos() + CPlayerInfo::GetInstance()->GetPosition();
	temp / 2;
	this->target.Lerp(temp, 5 * dt);
	this->position = this->target;
	this->position.z += 1;
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
    if (KeyboardController::GetInstance()->IsKeyDown('R'))
    {
        Reset();
    }
}

bool Camera::Deadzone(Vector3 *pos, Vector3 cameraTarget, float ortho)
{
    //Create a deadzone
    Vector3 upperRight;
    Vector3 bottomLeft;
    //They call me the mother of hardcoding hehe xd
    //Set the upperRight and bottomLeft Bounds

    ortho /= 100;

    upperRight.x = cameraTarget.x + (63 * ortho);
    upperRight.y = cameraTarget.y + (47 * ortho);


    bottomLeft.x = cameraTarget.x - (63 * ortho);
    bottomLeft.y = cameraTarget.y - (47 * ortho);

    //Position check
    if (pos->x < upperRight.x && pos->x > bottomLeft.x)
    {
        if (pos->y < upperRight.y && pos->y > bottomLeft.y)
        {
            //Player is within bounds

            return true;
        }
        //Player is out of bounds
        else
        {
            tempTarget = cameraTarget;

            return false;
        }
    }
    //player is out of bounds
    else
    {
        tempTarget = cameraTarget;

        return false;
    }
}

void Camera::Constrain(GenericEntity* constrain, float radius)
{
    if (this->target.x > constrain->GetPosition().x + radius)
        this->target.x = constrain->GetPosition().x + radius;
    if (this->target.x < constrain->GetPosition().x - radius)
        this->target.x = constrain->GetPosition().x - radius;
    if (this->target.y > constrain->GetPosition().y + radius)
        this->target.y = constrain->GetPosition().y + radius;
    if (this->target.y < constrain->GetPosition().y - radius)
        this->target.y = constrain->GetPosition().y - radius;

    this->position = this->target;
    this->position.z += 1;
}

void Camera::Constrain(CPlayerInfo p, Vector3 cameraTarget)
{
    Vector3 *u;
    Vector3 *b;

    u = new Vector3(cameraTarget.x + 10, cameraTarget.y + 10, 0);
    b = new Vector3(cameraTarget.x - 10, cameraTarget.y - 10, 0);

	if (p.GetPosition().x > u->x - 10 || p.GetPosition().y > u->y - 10 || p.GetPosition().x < b->x + 10 || p.GetPosition().y < b->y + 10)
    {
		if (p.GetPosition().x < u->x || p.GetPosition().y < u->y || p.GetPosition().x > b->x || p.GetPosition().y > b->y)
        {
			cameraTarget += p.GetPosition();
        }
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