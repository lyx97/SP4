#include "SpatialPartition.h"
#include "stdio.h"
#include "Collider/Collider.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "../Level/Heatmap.h"
#include "../Level/Level.h"

template <typename T> vector<T> concat(vector<T> &a, vector<T> &b) {
    vector<T> ret = vector<T>();
    copy(a.begin(), a.end(), back_inserter(ret));
    copy(b.begin(), b.end(), back_inserter(ret));
    return ret;
}

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
//CSpatialPartition *CSpatialPartition::sp_instance = 0;

/********************************************************************************
Constructor
********************************************************************************/
CSpatialPartition::CSpatialPartition(void)
    : theGrid(NULL)
    , xPos(0)
    , zPos(0)
    , xSize(0)
    , zSize(0)
    , xGridSize(0)
    , zGridSize(0)
    , xNumOfGrid(0)
    , zNumOfGrid(0)
    , yOffset(0.0f)
    , _meshName("")
    , theCamera(NULL)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CSpatialPartition::~CSpatialPartition(void)
{
    theCamera = NULL;
    delete[] theGrid;
}

/********************************************************************************
Initialise the spatial partition
********************************************************************************/
bool CSpatialPartition::Init(const int xGridSize, const int zGridSize,
    const int xNumOfGrid, const int zNumOfGrid, const int xPos, const int zPos,
    const float yOffset)
{
    if ((xGridSize>0) && (zGridSize>0)
        && (xNumOfGrid>0) && (zNumOfGrid>0))
    {
        this->xNumOfGrid = xNumOfGrid;
        this->zNumOfGrid = zNumOfGrid;
        this->xGridSize = xGridSize;
        this->zGridSize = zGridSize;
        this->xSize = xGridSize * xNumOfGrid;
        this->zSize = zGridSize * zNumOfGrid;
        this->xPos = xPos;
        this->zPos = zPos;
        this->yOffset = yOffset;
        // Create an array of grids
        theGrid = new CGrid[xNumOfGrid*zNumOfGrid];
        // Initialise the array of grids
        for (int i = 0; i<xNumOfGrid; i++)
        {
            for (int j = 0; j<zNumOfGrid; j++)
            {
                theGrid[i*zNumOfGrid + j].Init(
                    xGridSize*i - (xSize >> 1) + (xGridSize >> 1) + xPos, zGridSize*j - (zSize >> 1) + (zGridSize >> 1) + zPos,
                    i, j,
                    xGridSize, zGridSize,
                    0, 0
                    );
            }
        }
        // Assign a Mesh to each Grid if available.
        ApplyMesh();
        // Create a migration list vector
        MigrationList.clear();
        return true;
    }
    return false;
}

/********************************************************************************
Set a particular grid's Mesh
********************************************************************************/
void CSpatialPartition::SetMesh(const std::string& _meshName)
{
    this->_meshName = _meshName;

    // Assign a Mesh to each Grid if available.
    ApplyMesh();
}

/********************************************************************************
ApplyMesh
********************************************************************************/
void CSpatialPartition::ApplyMesh(void)
{
    if (_meshName.size() != 0)
    {
        for (int i = 0; i<xNumOfGrid; i++)
        {
            for (int j = 0; j<zNumOfGrid; j++)
            {
                theGrid[i*zNumOfGrid + j].SetMesh(_meshName);
            }
        }
    }
}

/********************************************************************************
Update the spatial partition
********************************************************************************/
void CSpatialPartition::Update(void)
{
    for (int i = 0; i<xNumOfGrid; i++)
    {
        for (int j = 0; j<zNumOfGrid; j++)
        {
            theGrid[i*zNumOfGrid + j].Update(&MigrationList);
        }
    }
    // If there are objects due for migration, then process them
    if (MigrationList.empty() == false)
    {
        // Check each object to see if they are no longer in this grid
		for (int i = 0; i < MigrationList.size(); ++i)
		{
			Add(MigrationList[i]);
		}
        MigrationList.clear();
    }
}

/********************************************************************************
Render the spatial partition
********************************************************************************/
void CSpatialPartition::Render(Vector3* theCameraPosition)
{
    //CHeatmap** temp = CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetHeatmap();
   
    // Render the Spatial Partitions
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(0.0f, yOffset, 0.0f);
    for (int i = 0; i<xNumOfGrid; i++)
    {
        for (int j = 0; j<zNumOfGrid; j++)
        {
            modelStack.PushMatrix();
            modelStack.Translate(xGridSize*i - (xSize >> 1) + (xGridSize >> 1) + xPos, 10.0f, zGridSize*j - (zSize >> 1) + (zGridSize >> 1) + zPos);
            modelStack.PushMatrix();
            modelStack.Scale(xGridSize, 1.0f, zGridSize);
            modelStack.Rotate(-90, 1, 0, 0);
            theGrid[i*zNumOfGrid + j].Render();
            modelStack.PopMatrix();
            modelStack.PopMatrix();
        }
    }
    modelStack.PopMatrix();
}

/********************************************************************************
Get xPos
********************************************************************************/
int CSpatialPartition::GetxPos(void) const
{
    return xPos;
}

/********************************************************************************
Get zPos
********************************************************************************/
int CSpatialPartition::GetzPos(void) const
{
    return zPos;
}

/********************************************************************************
Get xSize of the entire spatial partition
********************************************************************************/
int CSpatialPartition::GetxSize(void) const
{
    return xSize;
}
/********************************************************************************
Get zSize of the entire spatial partition
********************************************************************************/
int CSpatialPartition::GetzSize(void) const
{
    return zSize;
}
/********************************************************************************
Get xSize
********************************************************************************/
int CSpatialPartition::GetxGridSize(void) const
{
    return xGridSize;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int CSpatialPartition::GetzGridSize(void) const
{
    return zGridSize;
}
/********************************************************************************
Get xNumOfGrid
********************************************************************************/
int CSpatialPartition::GetxNumOfGrid(void) const
{
    return xNumOfGrid;
}
/********************************************************************************
Get zNumOfGrid
********************************************************************************/
int CSpatialPartition::GetzNumOfGrid(void) const
{
    return zNumOfGrid;
}

/********************************************************************************
Get a particular grid
********************************************************************************/
CGrid* CSpatialPartition::GetGrid(const int xIndex, const int zIndex) const
{
    return &theGrid[xIndex*zNumOfGrid + zIndex];
}

Vector3 CSpatialPartition::GetGridPos(const int xIndex, const int zIndex) const
{
    if (xIndex*zNumOfGrid + zIndex >= 0 && xIndex*zNumOfGrid + zIndex < xNumOfGrid * zNumOfGrid)
    {
        return theGrid[xIndex*zNumOfGrid + zIndex].GetPos();
    }
}

GRID_TYPE CSpatialPartition::GetGridType(const int xIndex, const int zIndex) const
{
    if (xIndex*zNumOfGrid + zIndex >= 0 && xIndex*zNumOfGrid + zIndex < xNumOfGrid * zNumOfGrid)
    {
        return theGrid[xIndex*zNumOfGrid + zIndex].GetType();
    }
}

void CSpatialPartition::SetGridType(const int xIndex, const int zIndex, const GRID_TYPE type)
{
    if (xIndex*zNumOfGrid + zIndex >= 0 && xIndex*zNumOfGrid + zIndex < xNumOfGrid * zNumOfGrid)
    {
        theGrid[xIndex*zNumOfGrid + zIndex].SetType(type);
    }
}


/********************************************************************************
Check frid for entity
********************************************************************************/

bool CSpatialPartition::CheckForEntity(const int xIndex, const int zIndex, EntityBase::ENTITY_TYPE type)
{
    if (xIndex*zNumOfGrid + zIndex >= 0 && xIndex*zNumOfGrid + zIndex < xNumOfGrid * zNumOfGrid)
    {
        if (theGrid[xIndex*zNumOfGrid + zIndex].IsHere(type))
            return true;
    }
    return false;
}

/********************************************************************************
Get entity from grid if present
********************************************************************************/

EntityBase* CSpatialPartition::GetEntity(const int xIndex, const int zIndex, EntityBase::ENTITY_TYPE type)
{
    if (xIndex*zNumOfGrid + zIndex >= 0 && xIndex*zNumOfGrid + zIndex < xNumOfGrid * zNumOfGrid)
    {
        return theGrid[xIndex*zNumOfGrid + zIndex].GetEntity(type);
    }
    return NULL;
}

/********************************************************************************
Get vector of objects from this Spatial Partition
********************************************************************************/
vector<EntityBase*> CSpatialPartition::GetObjects(const int xIndex, const int zIndex)
{
    // Get the indices of the object's position
    if (xIndex*zNumOfGrid + zIndex >= 0 && xIndex*zNumOfGrid + zIndex < xNumOfGrid * zNumOfGrid)
    {
        return theGrid[xIndex*zNumOfGrid + zIndex].GetListOfObject();
    }
}
vector<EntityBase*> CSpatialPartition::GetObjects(Vector3 position, const float radius)
{
    // Get the indices of the object's position
    int xIndex = (((int)position.x - (-xSize >> 1)) / (xSize / xNumOfGrid));
    int zIndex = (((int)position.z - (-zSize >> 1)) / (zSize / zNumOfGrid));

    return theGrid[xIndex*zNumOfGrid + zIndex].GetListOfObject();
}

/********************************************************************************
Add a new object model
********************************************************************************/
void CSpatialPartition::Add(EntityBase* theObject)
{
    // Get the indices of the object's position
    int xIndex = (((int)theObject->GetPosition().x + (xSize >> 1) - xPos) / xGridSize);
    int zIndex = (((int)theObject->GetPosition().z + (zSize >> 1) - zPos) / zGridSize);
    // Add them to each grid
    if (((xIndex >= 0) && (xIndex < xNumOfGrid)) && ((zIndex >= 0) && (zIndex < zNumOfGrid)))
    {
        theObject->SetIndex(xIndex, zIndex);
        theGrid[xIndex*zNumOfGrid + zIndex].Add(theObject);
    }
}

// Remove but not delete object from this grid
void CSpatialPartition::Remove(EntityBase* theObject)
{
    // Get the indices of the object's position
    int xIndex = (((int)theObject->GetPosition().x + (xSize >> 1) - xPos) / xGridSize);
    int zIndex = (((int)theObject->GetPosition().z + (zSize >> 1) - zPos) / zGridSize);
    // Add them to each grid
    if (((xIndex >= 0) && (xIndex<xNumOfGrid)) && ((zIndex >= 0) && (zIndex<zNumOfGrid)))
    {
        theGrid[xIndex*zNumOfGrid + zIndex].Remove(theObject);
    }
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float CSpatialPartition::CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int zIndex)
{
    float xDistance = (xGridSize * xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition->x;
    float zDistance = (zGridSize * zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition->z;

    return (float)(xDistance*xDistance + zDistance*zDistance);
}

/********************************************************************************
Set Camera
********************************************************************************/
void CSpatialPartition::SetCamera(Camera* _cameraPtr)
{
    this->theCamera = _cameraPtr;
}

/********************************************************************************
Remove Camera
********************************************************************************/
void CSpatialPartition::RemoveCamera(void)
{
    this->theCamera = NULL;
}

/********************************************************************************
Check if a CGrid is visible to the camera
********************************************************************************/
bool CSpatialPartition::IsVisible(Vector3 theCameraPosition, Vector3 theCameraDirection, const int xIndex, const int zIndex)
{
    float xDistance = (xGridSize * xIndex + (xGridSize >> 1) - (xSize >> 1)) - theCameraPosition.x;
    float zDistance = (zGridSize * zIndex + (zGridSize >> 1) - (zSize >> 1)) - theCameraPosition.z;
    // If the camera is within the CGrid, then display by default
    // Otherwise, the entity may not get displayed
    if (xDistance * xDistance + zDistance * zDistance < (xGridSize * xGridSize + zGridSize * zGridSize))
        return true;
    Vector3 gridCentre(xDistance, 0, zDistance);
    if (theCameraDirection.Dot(gridCentre) < 0)
        return false;
    return true;
}

/********************************************************************************
PrintSelf
********************************************************************************/
void CSpatialPartition::PrintSelf() const
{
    cout << "******* Start of CSpatialPartition::PrintSelf() **********************************" << endl;
    cout << "xSize\t:\t" << xSize << "\tzSize\t:\t" << zSize << endl;
    cout << "xNumOfGrid\t:\t" << xNumOfGrid << "\tzNumOfGrid\t:\t" << zNumOfGrid << endl;
    if (theGrid)
    {
        cout << "theGrid : OK" << endl;
        cout << "Printing out theGrid below: " << endl;
        for (int i = 0; i<xNumOfGrid; i++)
        {
            for (int j = 0; j<zNumOfGrid; j++)
            {
                theGrid[i*zNumOfGrid + j].PrintSelf();
            }
        }
    }
    else
        cout << "theGrid : NULL" << endl;
    cout << "******* End of CSpatialPartition::PrintSelf() **********************************" << endl;
}
