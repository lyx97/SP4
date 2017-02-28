#pragma once

#include "Vector3.h"
#include "Grid.h"
#include "EntityBase.h"
#include "../Camera/Camera.h"

class CHeatmap;
class CLevel;

class CSpatialPartition
{
protected:
	//static CSpatialPartition *sp_instance;
	// Constructor
	//CSpatialPartition(void);

	// Variables
	CGrid* theGrid;
	int xSize;
	int zSize;
	int xGridSize;
	int zGridSize;
	int xNumOfGrid;
	int zNumOfGrid;
    int xPos;
    int zPos;
	float yOffset;
	std::string _meshName; // Name of the mesh

    // We store the pointer to the Camera so we can get it's position and direction to calculate LOD and visibility
    Camera* theCamera;

public:
	//static CSpatialPartition *GetInstance()
	//{
	//	if (!sp_instance)
	//		sp_instance = new CSpatialPartition;
	//	return sp_instance;
	//}
	//static bool DropInstance()
	//{
	//	if (sp_instance)
	//	{
	//		delete sp_instance;
	//		sp_instance = NULL;
	//		return true;
	//	}
	//	return false;
	//}
    CSpatialPartition(void);
	~CSpatialPartition(void);

	// Initialise the spatial partition
	bool Init(	const int xGridSize, const int zGridSize, 
				const int xNumOfGrid, const int zNumOfGrid, 
                const int xPos, const int zPos,
				const float yOffset = -9.9f);

	// Set a particular grid's Mesh
	void SetMesh(const std::string& _meshName);

	// ApplyMesh
	void ApplyMesh(void);

	// Update the spatial partition
	void Update(void);
	// Render the spatial partition
	void Render(Vector3* theCameraPosition = NULL);

    // Get xPos
    int GetxPos(void) const;
    // Get zPos
    int GetzPos(void) const;
	// Get xSize of the entire spatial partition
	int GetxSize(void) const;
	// Get zSize of the entire spatial partition
	int GetzSize(void) const;
	// Get xSize
	int GetxGridSize(void) const;
	// Get zNumOfGrid
	int GetzGridSize(void) const;
	// Get xNumOfGrid
	int GetxNumOfGrid(void) const;
	// Get zNumOfGrid
	int GetzNumOfGrid(void) const;

	// Get a particular grid
	CGrid* GetGrid(const int xIndex, const int zIndex) const;
    Vector3 GetGridPos(const int xIndex, const int zIndex) const;
    GRID_TYPE GetGridType(const int xIndex, const int zIndex) const;
    void SetGridType(const int xIndex, const int zIndex, const GRID_TYPE type);

    // Check if entity is present
    bool CheckForEntity(const int xIndex, const int zIndex, EntityBase::ENTITY_TYPE type);
    // Get entity if present
    EntityBase* GetEntity(const int xIndex, const int zIndex, EntityBase::ENTITY_TYPE type);

    // Get vector of objects from this Spatial Partition
    vector<EntityBase*> GetObjects(const int xIndex, const int zIndex);
    vector<EntityBase*> GetObjects(Vector3 position, const float radius);

	// Add a new object
	void Add(EntityBase* theObject);
	// Remove but not delete object from this grid
	void Remove(EntityBase* theObject);

	// Calculate the squared distance from camera to a grid's centrepoint
	float CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int zIndex);

	//PrintSelf
	void PrintSelf() const;

	// The vector of objects due for migration to another grid
	vector<EntityBase*> MigrationList;

    // Handling Camera
    void SetCamera(Camera* _cameraPtr);
    void RemoveCamera(void);

    // Check if a CGrid is visible to the camera
    bool IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int zIndex);
};
