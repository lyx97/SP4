#include "Grid.h"
#include "stdio.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "../GenericEntity.h"

/********************************************************************************
Constructor
********************************************************************************/
CGrid::CGrid(void)
    : position(Vector3(-1, -1, -1))
    , index(Vector3(-1, -1, -1))
    , size(Vector3(-1, -1, -1))
    , offset(Vector3(-1, -1, -1))
    , min(Vector3(-1, -1, -1))
    , max(Vector3(-1, -1, -1))
    , theMesh(NULL)
    , ListOfObjects(NULL)
    , m_eGridType(DEFAULT)
{
}

/********************************************************************************
Destructor
********************************************************************************/
CGrid::~CGrid(void)
{
    if (theMesh)
    {
        delete theMesh;
        theMesh = NULL;
    }
    Remove();
}

/********************************************************************************
Initialise this grid
********************************************************************************/
void CGrid::Init(const int xPosition, const int zPosition,
    const int xIndex, const int zIndex,
    const int xGridSize, const int zGridSize,
    const float xOffset, const float zOffset)
{
    position.Set(xPosition, 0, zPosition);
    index.Set(xIndex, 0, zIndex);
    size.Set(xGridSize, 0, zGridSize);
    offset.Set(xOffset, 0, zOffset);
    min.Set(position.x - (xGridSize >> 1), 0.0f, position.z - (zGridSize >> 1));
    max.Set(position.x + (xGridSize >> 1), 0.0f, position.z + (zGridSize >> 1));
}

/********************************************************************************
Set a particular grid's Mesh
********************************************************************************/
void CGrid::SetMesh(const std::string& _meshName)
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh != nullptr)
    {
        theMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    }
}

/********************************************************************************
Update the grid
********************************************************************************/
void CGrid::Update(vector<EntityBase*>* migrationList)
{
    // Check each object to see if they are no longer in this grid
    std::vector<EntityBase*>::iterator it;
    it = ListOfObjects.begin();
    while (it != ListOfObjects.end())
    {
        if (!(*it)->IsDone())
        {
            Vector3 position = (*it)->GetPosition();
            if (((min.x <= position.x) && (position.x <= max.x)) &&
                ((min.z <= position.z) && (position.z <= max.z)))
            {
                // Move on otherwise
                ++it;
            }
            else
            {
                migrationList->push_back(*it);
                // Remove from this Grid
                it = ListOfObjects.erase(it);
            }
        }
        else
            ++it;
    }
}

/********************************************************************************
RenderScene
********************************************************************************/
void CGrid::Render(void)
{
    //if (m_bIsPath)
    //    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("trail"));
    //else if (theMesh && m_bIsRoom)
    //{
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    RenderHelper::RenderMesh(theMesh);
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}
    //else if (m_bIsWall)
    //    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("influence"));

    //if (theMesh)
    //if (m_eGridType == GRID_TYPE::PATH)
    //{
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    //RenderHelper::RenderMesh(theMesh);
    //    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GRIDMESH"));
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}

    //MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    //modelStack.PushMatrix();
    ////CHeatmap** temp = CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetHeatmap();
    ////
    ////int tempX = index.x;
    ////int tempZ = index.z;
    ////Vector3 dir = temp[tempX][tempZ].GetDir();

    ////modelStack.Translate(position.x, 0, position.z);
    //modelStack.Rotate(-90, 1, 0, 0);
    ////modelStack.Rotate(Math::RadianToDegree(atan2(dir.x, dir.z)), 0, 0, 1);
    //modelStack.Scale(size.x, size.y, size.z);
    //RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GRIDMESH"));
    //modelStack.PopMatrix();

    //MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    //modelStack.PushMatrix();
    //modelStack.Translate(position.x, position.y, position.z);
    //modelStack.Rotate(-90, 1, 0, 0);
    //modelStack.Scale(size.x, size.y, size.z);
    //RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GRIDMESH"));;
    //modelStack.PopMatrix();

    if (m_eGridType == GRID_TYPE::WALL)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //RenderHelper::RenderMesh(theMesh);
        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("wall"));
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    //else
    //{
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("direction"));
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}
}

/********************************************************************************
RenderObjects
********************************************************************************/
void CGrid::RenderObjects(const int RESOLUTION)
{
}

/********************************************************************************
Add a new object to this grid
********************************************************************************/
void CGrid::Add(EntityBase* theObject)
{
    for (int i = 0; i < ListOfObjects.size(); ++i)
    {
        if (ListOfObjects[i] == theObject)
            return;
    }
    ListOfObjects.push_back(theObject);
}

/********************************************************************************
Kick object out of grid
********************************************************************************/
bool CGrid::Kick(EntityBase* theObject)
{
    // Clean up entities that are done
    std::vector<EntityBase*>::iterator it, end;
    it = ListOfObjects.begin();
    end = ListOfObjects.end();
    while (it != end)
    {
        if ((*it) == theObject)
        {
            it = ListOfObjects.erase(it);
            return true;
        }
        else
        {
            // Move on otherwise
            ++it;
        }
    }
    return false;
}

/********************************************************************************
Remove but not delete object from this grid
********************************************************************************/
void CGrid::Remove(void)
{
    for (int i = 0; i < ListOfObjects.size(); i++)
    {
        // Do not delete the objects as they are stored in EntityManager and will be deleted there.
        //delete ListOfObjects[i];
        ListOfObjects[i] = NULL;
    }
    ListOfObjects.clear();
}

/********************************************************************************
Remove but not delete an object from this grid
********************************************************************************/
bool CGrid::Remove(EntityBase* theObject)
{
    // Clean up entities that are done
    std::vector<EntityBase*>::iterator it, end;
    it = ListOfObjects.begin();
    end = ListOfObjects.end();
    while (it != end)
    {
        if ((*it) == theObject)
        {
            it = ListOfObjects.erase(it);
            return true;
        }
        else
        {
            // Move on otherwise
            ++it;
        }
    }
    return false;
}

/********************************************************************************
Check if an object is in this grid
********************************************************************************/
bool CGrid::IsHere(EntityBase* theObject) const
{
    for (int i = 0; i < ListOfObjects.size(); ++i)
    {
        if (ListOfObjects[i] == theObject)
            return true;
    }
    return false;
}

bool CGrid::IsHere(EntityBase::ENTITY_TYPE entityType) const
{
    for (int i = 0; i < ListOfObjects.size(); ++i)
    {
        if (!ListOfObjects[i]->IsDone())
        {
            if (ListOfObjects[i]->GetEntityType() == entityType)
                return true;
        }
    }
    return false;
}

EntityBase* CGrid::GetEntity(EntityBase::ENTITY_TYPE entityType) const
{
    for (int i = 0; i < ListOfObjects.size(); ++i)
    {
        if (!ListOfObjects[i]->IsDone())
        {
            if (ListOfObjects[i]->GetEntityType() == entityType)
                return ListOfObjects[i];
        }
    }
    return NULL;
}

/********************************************************************************
Check if position is in this grid
********************************************************************************/
bool CGrid::PointInGrid(const int xPos, const int zPos) const
{
    if (((min.x <= xPos) && (xPos <= max.x)) &&
        ((min.z <= zPos) && (zPos <= max.z)))
    {
        return true;
    }
    return false;
}

/********************************************************************************
Get list of objects in this grid
********************************************************************************/
vector<EntityBase*> CGrid::GetListOfObject(void)
{
    return ListOfObjects;
}

Vector3 CGrid::GetPos(void) const
{
    return this->position;
}

GRID_TYPE CGrid::GetType(void) const
{
    return this->m_eGridType;
}

void CGrid::SetType(const GRID_TYPE type)
{
    this->m_eGridType = type;
}

/********************************************************************************
PrintSelf
********************************************************************************/
void CGrid::PrintSelf()
{
    cout << "CGrid::PrintSelf()" << endl;
    cout << "\tIndex\t:\t" << index << "\t\tOffset\t:\t" << offset << endl;
    cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
    if (ListOfObjects.size() > 0)
    {
        cout << "\t------------------------------------------------------------------------" << endl;
    }
    for (int i = 0; i < ListOfObjects.size(); ++i)
    {
        cout << "\t" << i << "\t:\t" << ListOfObjects[i]->GetPosition() << endl;
    }
    if (ListOfObjects.size()>0)
        cout << "\t------------------------------------------------------------------------" << endl;
    cout << "********************************************************************************" << endl;
}
