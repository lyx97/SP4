#pragma once

#include "../GenericEntity.h"
#include "../Waypoint/Waypoint.h"

class Mesh;
class CLevel;
class CPlayerInfo;

class CEnemy : public GenericEntity
{
protected:

    Vector3 defaultPosition, defaultTarget, defaultUp;
    Vector3 target, up;
    Vector3 maxBoundary, minBoundary;

	// Vector containing IDs of Waypoints
	vector<int> listOfWaypoints;
	// Current ID of Waypoint
	int m_iWayPointIndex;

	double m_dSpeed;
	double m_dAcceleration;

public:
    CEnemy(void);
    virtual ~CEnemy(void);

    void Init(void);

    // Reset this player instance to default
    void Reset(void);
    // Set position
    void SetPos(const Vector3& pos);
    // Set target
    void SetTarget(const Vector3& target);
    // Set Up
    void SetUp(const Vector3& up);
    // Set the boundary for the player info
    void SetBoundary(Vector3 max, Vector3 min);
    // Get next Waypoint for this CEnemy
    CWaypoint* GetNextWaypoint(void);

    // Get position
    Vector3 GetPos(void) const;
    // Get target
    Vector3 GetTarget(void) const;
    // Get Up
    Vector3 GetUp(void) const;

    // Update
    void Update(double dt = 0.0333f);

    // Constrain the position within the borders
    void Constrain(void);
    // Render
    void Render(float& _renderOrder);
};