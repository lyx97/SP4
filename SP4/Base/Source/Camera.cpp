#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera() :
position(Vector3(0, 0, 1)),
target(Vector3(0, 0, 0)),
up(Vector3(0, 1, 0))
{
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;

	m_worldHeight = 100.0f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	isDeadzoned = false;
}

void Camera::Update(double dt)
{
	//Retrieve Entities from vector<Vector3>entityList and update the camera position based off their average pos
	Vector3 cameraTarget;

	//null check
	if (!entityList.empty())
	{
		//Calculate Camera Position
		for (int i = 0; i < entityList.size(); i++)
		{
			//Add all the positions
			cameraTarget.x += (entityList[i]->x);
			cameraTarget.y += (entityList[i]->y);
		}
		//Average the positions
		cameraTarget.x /= entityList.size();
		cameraTarget.y /= entityList.size();

		//if so, update the camera position
		this->target.Lerp(cameraTarget, 5 * dt);
		this->position = this->target;
		this->position.z += 1;
	}
}

void Camera::Constrain(GameObject* constrain, float radius)
{
	if (this->target.x > constrain->pos.x + radius)
		this->target.x = constrain->pos.x + radius;
	if (this->target.x < constrain->pos.x - radius)
		this->target.x = constrain->pos.x - radius;
	if (this->target.y > constrain->pos.y + radius)
		this->target.y = constrain->pos.y + radius;
	if (this->target.y < constrain->pos.y - radius)
		this->target.y = constrain->pos.y - radius;

	this->position = this->target;
	this->position.z += 1;
}

void Camera::Include(Vector3* pos)
{
	entityList.push_back(pos);
}

Vector3 Camera::GetPosition()
{
	return this->position;
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

void Camera::Constrain(Player p, Vector3 cameraTarget)
{
	Vector3 *u;
	Vector3 *b;

	u = new Vector3(cameraTarget.x + 10, cameraTarget.y + 10, 0);
	b = new Vector3(cameraTarget.x - 10, cameraTarget.y - 10, 0);

	if (p.GetPosition().x > u->x - 10 || p.GetPosition().y > u->y - 10 || p.GetPosition().x < b->x + 10 || p.GetPosition().y < b->y + 10)
	{
		if (p.GetPosition().x < u->x || p.GetPosition().y < u->y || p.GetPosition().x > b->x || p.GetPosition().y > b->y)
		{
			cameraTarget += p.GetVelocity();
		}
	}
}