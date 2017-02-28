#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Waypoint/WaypointManager.h"
#include "MeshBuilder.h"
#include "../Level/Level.h"
#include "../PlayerInfo/PlayerInfo.h"

CEnemy::CEnemy(void)
    : GenericEntity(NULL)
    , defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
    , defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
    , defaultUp(Vector3(0.0f, 0.0f, 0.0f))
    , target(Vector3(0.0f, 0.0f, 0.0f))
    , up(Vector3(0.0f, 0.0f, 0.0f))
    , maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
    , minBoundary(Vector3(0.0f, 0.0f, 0.0f))
    , m_iWayPointIndex(-1)
{
    listOfWaypoints.clear();
}

CEnemy::~CEnemy(void)
{

}

void CEnemy::Init(void)
{
    // Set the default values
    defaultPosition.Set(0, 0, 10);
    defaultTarget.Set(0, 0, 0);
    defaultUp.Set(0, 1, 0);

    // Set up the waypoints
    //listOfWaypoints.push_back(0);
    //listOfWaypoints.push_back(1);
    //listOfWaypoints.push_back(2);

    m_iWayPointIndex = 0;

    // Set the current values
    position.Set(100.0f, 0.0f, 1000.0f);
    //target.Set(10.0f, 0.0f, 450.0f);
    //CWaypoint* nextWaypoint = GetNextWaypoint();
    //if (nextWaypoint)
    //    target = nextWaypoint->GetPosition();
    //else
    //    target = Vector3(0, 0, 0);
    //cout << "Next target: " << target << endl;
    up.Set(0.0f, 1.0f, 0.0f); 
    // Set Boundary
    maxBoundary.Set(1, 1, 1);
    minBoundary.Set(-1, -1, -1);
    // Set speed
    m_dSpeed = 1.0;
    // Set Entity Type
    m_eEntityType = EntityBase::ENEMY;
    // Initialise the Collider
    this->SetCollider(true);
    this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
    // Add to EntityManager
    EntityManager::GetInstance()->AddEntity(this, CPlayerInfo::GetInstance()->GetRoomID());
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
    // Set the current values to default values
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
    position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
    this->target = target;
}

// Set Up
void CEnemy::SetUp(const Vector3& up)
{
    this->up = up;
}

// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
    maxBoundary = max;
    minBoundary = min;
}


// Get position
Vector3 CEnemy::GetPos(void) const
{
    return this->position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
    return this->target;
}

// Get Up
Vector3 CEnemy::GetUp(void) const
{
    return this->up;
}

// Get next Waypoint for this CEnemy
CWaypoint* CEnemy::GetNextWaypoint(void)
{
    if ((int)listOfWaypoints.size() > 0)
    {
        m_iWayPointIndex++;
        if (m_iWayPointIndex >= (int)listOfWaypoints.size())
            m_iWayPointIndex = 0;
        return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
    }
    else
        return NULL;
}

// Update
void CEnemy::Update(double dt)
{
    // cout << position << "..." << viewVector << endl;
	position += velocity * dt * 10;
	
    // Constrain the position
    //Constrain();

    // Update the target
    //if (position.z > 400.0f)
    //    target.z = position.z * -1;
    //else if (position.z < -400.0f)
    //    target.z = position.z * -1;

    //if ((target - position).LengthSquared() < 25.0f)
    //{
    //    CWaypoint* nextWaypoint = GetNextWaypoint();
    //    if (nextWaypoint)
    //        target = nextWaypoint->GetPosition();
    //    else
    //        target = Vector3(0, 0, 0);
    //    cout << "Next target: " << target << endl;
    //}
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
    if (position.x > maxBoundary.x - 1.0f)
        position.x = maxBoundary.x - 1.0f;
    if (position.z > maxBoundary.z - 1.0f)
        position.z = maxBoundary.z - 1.0f;
    if (position.x < minBoundary.x + 1.0f)
        position.x = minBoundary.x + 1.0f;
    if (position.z < minBoundary.z + 1.0f)
        position.z = minBoundary.z + 1.0f;
}

// Render
void CEnemy::Render(float& _renderOrder)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y + _renderOrder, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("cube"));;
    modelStack.PopMatrix();
}