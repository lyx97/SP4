#include "Laser.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL/glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Level/Level.h"
#include "../BatchRendering.h"

#include <iostream>
using namespace std;

CLaser::CLaser(void)
	: GenericEntity(NULL)
    , prevIndex(Vector3(0, 0, 0))
	, m_fLength(0.0)
	, angle_x(0.0)
	, angle_y(0.0)
	, angle_z(0.0)
{
    // Particle
    color.Set(Vector3(1.f, 0.f, 0.f));
}

CLaser::CLaser(Mesh* _modelMesh)
	: GenericEntity(_modelMesh)
    , prevIndex(Vector3(0, 0, 0))
	, m_fLength(0.0)
	, angle_x(0.0)
	, angle_y(0.0)
	, angle_z(0.0)
{
    // Particle
    color.Set(Vector3(1.f, 0.f, 0.f));
}

CLaser::~CLaser(void)
{
}

// Set the length of the laser
void CLaser::SetLength(const float m_fLength)
{
	this->m_fLength = m_fLength;
}

// Get the length of the laser
float CLaser::GetLength(void) const
{
	return m_fLength;
}

// Update this laser
void CLaser::CalculateAngles(void)
{
	angle_x = acos(theDirection.x / theDirection.Length());
	if ((theDirection.x < 0) && (theDirection.z > 0))
		angle_x *= -1.0f;
	angle_y = acos(theDirection.y / theDirection.Length());
	angle_z = acos(theDirection.z / theDirection.Length());
	if ((theDirection.z < 0) && (theDirection.x < 0))
		angle_z *= -1.0f;
	if ((theDirection.z > 0) && (theDirection.x < 0))
		angle_z *= -1.0f;
}

// Update the status of this projectile
void CLaser::Update(double dt)
{
	if (m_bStatus == false)
		return;

    // Update TimeLife of projectile. Set to inactive if too long
    m_fLifetime -= (float)dt;
    if (m_fLifetime < 0.0f)
    {
        m_bStatus = false;
        SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
        BatchRendering::GetInstance()->GetParticle(
            position,
            Vector3(2.f, 2.f, 2.f),
            color,
            Vector3(0.f, 0.f, 0.f),
            NUMPARTICLESPAWN);
        return;
    }

    if (prevIndex != index)
    {
        if (CLevel::GetInstance()->
            GetRoom(CPlayerInfo::GetInstance()
            ->GetRoomID())
            ->GetSpatialPartition()
            ->GetGridType(index.x, index.z)
            == GRID_TYPE::WALL)
        {
            m_bStatus = false;
            SetIsDone(true);
            BatchRendering::GetInstance()->GetParticle(
                position,
                Vector3(2.f, 2.f, 2.f),
                Vector3(0.f, 0.8f, 1.f),
                theDirection* dt * m_fSpeed,
                NUMPARTICLESPAWN);

            BatchRendering::GetInstance()->GetParticle(
                position,
                Vector3(2.f, 2.f, 2.f),
                color,
                -theDirection* dt * m_fSpeed,
                NUMPARTICLESPAWN);
            return;
        }
        prevIndex = index;
    }

	// Update Position
	position.Set(position.x + (float)(theDirection.x * dt * m_fSpeed),
				position.y + (float)(theDirection.y * dt * m_fSpeed),
				position.z + (float)(theDirection.z * dt * m_fSpeed));
}

// Render this projectile
void CLaser::Render(float& _renderOrder)
{
	if (m_bStatus == false)
		return;

	if (m_fLifetime < 0.0f)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	// Reset the model stack
	modelStack.LoadIdentity();
	// We introduce a small offset to y position so that we can see the laser beam.
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
	//modelStack.Scale(scale.x, scale.y, scale.z);
	modelStack.PushMatrix();
	modelStack.Rotate(180 / Math::PI * angle_z, 0.0f, 1.0f, 0.0f);
	modelStack.PushMatrix();
	//modelStack.Rotate(180 / Math::PI * angle_x, 1.0f, 0.0f, 0.0f);	// Not needed!
	modelStack.PushMatrix();
	modelStack.Rotate(180 / Math::PI * angle_y, 1.0f, 0.0f, 0.0f);
	glLineWidth(5.0f);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("laser"));
	glLineWidth(1.0f);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void CLaser::Set(Vector3 theNewPosition, Vector3 theNewDirection, const float m_fLifetime, const float m_fSpeed)
{
	position = theNewPosition;
	theDirection = theNewDirection;
	this->m_fLifetime = m_fLifetime;
	this->m_fSpeed = m_fSpeed;
}

// Create a projectile and add it into EntityManager
CLaser* Create::Laser(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _direction,
	const float m_fLength,
	const float m_fLifetime,
	const float m_fSpeed,
	const float m_fDamage)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CLaser* result = new CLaser(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetLength(m_fLength);
	result->SetDamage(m_fDamage);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetIsLaser(true);
	result->SetMass(8);
	result->SetAABB(Vector3(0.5f, 0.5f, 0.5), Vector3(-0.5f, -0.5f, -0.5f));
	result->SetEntityType(EntityBase::PROJECTILE);
	result->SetVelocity(_direction); // to get the velocity outside the class
    result->SetRoomID(CPlayerInfo::GetInstance()->GetRoomID());
    EntityManager::GetInstance()->AddEntity(result, CPlayerInfo::GetInstance()->GetRoomID());

	result->CalculateAngles();
	return result;
}