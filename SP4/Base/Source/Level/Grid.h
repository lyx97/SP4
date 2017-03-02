#pragma once

#include "EntityBase.h"
#include "Vector3.h"
#include <vector>
using namespace std;

//Include GLEW
#include <GL/glew.h>

class Mesh;
class CPlayerInfo;
class CLevel;

enum GRID_TYPE
{
    DEFAULT,
    PATH,
    WALL,
    DOORPATH,
    DOOR,
    STAIR,
    OBSTACLE,
    NUM_GRIDTYPE
};

class CGrid
{
protected:
    GRID_TYPE m_eGridType;

    Vector3 position;
	// We use a Vector3 to store the indices of this Grid within the Spatial Partition array.
	Vector3 index;
	// We use a Vector3 to store the size of this Grid within the Spatial Partition array.
	Vector3 size;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 offset;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 min, max;

	// The mesh to represent the grid
	Mesh* theMesh;

	// List of objects in this grid
	vector<EntityBase*> ListOfObjects;

public:
	// Constructor
	CGrid(void);
	// Destructor
	~CGrid(void);

	// Init
    void Init(const int xPosition, const int zPosition,
        const int xIndex, const int zIndex,
        const int xGridSize, const int zGridSize,
        const float xOffset = 0, const float zOffset = 0);

	// Set a particular grid's Mesh
	void SetMesh(const std::string& _meshName);

	// Update the grid
	void Update(vector<EntityBase*>* migrationList);
	// Render the grid
	void Render(void);
	// RenderObjects
	void RenderObjects(const int RESOLUTION);

	// Add a new object to this grid
	void Add(EntityBase* theObject);
    // Kick object out of grid
    bool Kick(EntityBase* theObject);
	// Remove but not delete all objects from this grid
	void Remove(void);
	// Remove but not delete an object from this grid
	bool Remove(EntityBase* theObject);

    // Check if an object is in this grid
    bool IsHere(EntityBase* theObject) const;
    bool IsHere(EntityBase::ENTITY_TYPE entityType) const;
    EntityBase* GetEntity(EntityBase::ENTITY_TYPE theObject) const;

    // Check if position is in this grid
    bool PointInGrid(const int xPos, const int zPos) const;

	// Get list of objects in this grid
	vector<EntityBase*> GetListOfObject(void);

    Vector3 GetPos(void) const;

    inline Vector3 GetMin(void) const { return min; }
    inline Vector3 GetMax(void) const { return max; }

    // Type
    GRID_TYPE GetType(void) const;
    void SetType(const GRID_TYPE type);

	// PrintSelf
	void PrintSelf();
};
