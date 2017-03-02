#include "Level.h"
#include "../Application.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "../PlayerInfo/PlayerInfo.h"

CLevel::CLevel()
    : m_iRoomID(1)
    , m_fRoomBias(0.f)
    , roomIndexList(NULL)
    , roomList(NULL)
{
}

CLevel::~CLevel()
{

}

void CLevel::Init(const float room_bias)
{
    roomIndexList.clear();
    roomNumEnemyList.clear();
    roomList.clear();
    roomMap.clear();

    m_iRoomID = 0;
    m_fRoomBias = room_bias;

    cout << "LEVEL: " << m_iLevel << endl;

    roomIndexList.push_back(Vector3(0, 0, 0));

    CreateRoom(m_iRoomID, 0, 0, 0, 0, true);

    ExpandRoom();

    // Add special room limited to each level
    AddSpecialRoom(ROOMSIZE, ROOMSIZE, ROOM_TYPE::TREASUREROOM);
    AddSpecialRoom(ROOMSIZE, ROOMSIZE, ROOM_TYPE::NEXTLEVELROOM);

    for (int i = 0; i < roomIndexList.size(); ++i)
    {
        cout << "Room: " << i << " Index: " << roomIndexList[i] << endl;
    }
    cout << "roomlist size: " << roomList.size() << endl;

    // Link rooms
    for (auto it : roomList)
    {
        SetDoor(it);
    }

    // Check if room has enemy and set doors open if none
    int count = 0;
    for (auto it : roomNumEnemyList)
    {
        if (it == 0)
            CLevel::GetInstance()->GetRoom(count)->SetRoomCleared();
        count++;
    }

    m_iLevel++;
}

void CLevel::Render()
{
    CRoom* room = GetRoom(CPlayerInfo::GetInstance()->GetRoomID());

    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    if (room->GetDoorToRoomID(0) >= 0)
    {
        modelStack.PushMatrix();
        modelStack.Translate(-((room->GetRoomXMax() * GRIDSIZE) >> 1), -0.1f, 0);
        modelStack.Rotate(90, -1, 0, 0);
        modelStack.Scale(GRIDSIZE, GRIDSIZE, 1);
        if (room->GetRoomCleared())
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dooropen2"));
        else
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("doorlocked2"));
        modelStack.PopMatrix();
    }

    if (room->GetDoorToRoomID(1) >= 0)
    {
        modelStack.PushMatrix();
        modelStack.Translate(((room->GetRoomXMax() * GRIDSIZE) >> 1), -0.1f, 0);
        modelStack.Rotate(90, -1, 0, 0);
        modelStack.Rotate(180, 0, 0, 1);
        modelStack.Scale(GRIDSIZE, GRIDSIZE, 1);
        if (room->GetRoomCleared())
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dooropen2"));
        else
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("doorlocked2"));
        modelStack.PopMatrix();
    }

    if (room->GetDoorToRoomID(2) >= 0)
    {
        modelStack.PushMatrix();
        modelStack.Translate(0, -0.1f, -((room->GetRoomZMax() * GRIDSIZE) >> 1));
        modelStack.Rotate(90, -1, 0, 0);
        modelStack.Scale(GRIDSIZE, GRIDSIZE, 1);
        if (room->GetRoomCleared())
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dooropen2"));
        else
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("doorlocked"));
        modelStack.PopMatrix();
    }

    if (room->GetDoorToRoomID(3) >= 0)
    {
        modelStack.PushMatrix();
        modelStack.Translate(0, -0.1f, ((room->GetRoomZMax() * GRIDSIZE) >> 1));
        modelStack.Rotate(90, -1, 0, 0);
        modelStack.Scale(GRIDSIZE, GRIDSIZE, 1);
        if (room->GetRoomCleared())
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("dooropen2"));
        else
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("doorlocked"));
        modelStack.PopMatrix();
    }

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    if (room->GetRoomType() == ROOM_TYPE::NEXTLEVELROOM)
    {
        modelStack.PushMatrix();
        modelStack.Rotate(90, -1, 0, 0);
        modelStack.Scale(GRIDSIZE, GRIDSIZE, 1);
        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("stair"));
        modelStack.PopMatrix();
    }

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

const bool CLevel::CompareOverlap(Vector3 index)
{
    int tempIter = 0;
    while (tempIter < roomIndexList.size())
    {
        if (roomIndexList[tempIter] == index)
            return true;
        tempIter++;
    }
    return false;
}

CRoom* CLevel::GetRoom(const int roomID)
{
    if (roomList.size() > 0)
        return roomList[roomID];
    return NULL;
}

int CLevel::GetRoom(const int xIndex, const int zIndex)
{
    int tempIter = 0;
    while (tempIter < roomIndexList.size())
    {
        if (roomIndexList[tempIter] == Vector3(xIndex, 0, zIndex))
            return tempIter;
        tempIter++;
    }
    return -1;
}

void CLevel::CreateRoom(const int roomID,
                        const int xSize, const int zSize,
                        const int xIndex, const int zIndex,
                        bool firstRoom)
{
    // Create and add the room
    CRoom* room = new CRoom();
    if (firstRoom)
        room->Add(m_iRoomID, ROOMSIZE, ROOMSIZE, xIndex, zIndex, ROOM_TYPE::STARTROOM);
    else
        room->Add(m_iRoomID, xSize, zSize, xIndex, zIndex, ROOM_TYPE::ENEMYROOM);

    // Create room mapping
    roomMap[roomID];

    // random room for expansion
    // check if room can be expanded
    int tempRoomID = expandList.size() + roomList.size();
    if (roomList.size() > 0)
        tempRoomID--;

    for (int i = 0; i < 4; ++i)
    {
        roomMap[roomID][i] = -1;
        if (m_fRoomBias >= Math::RandFloatMinMax(0,100))
        {
            // Init
            Vector3 ExpandListData = Vector3(0, 0, 0);

            // Reduce room expand chance
            m_fRoomBias *= 0.75f;

            // Set Index for room mapping & Check for overlap
            bool AddToExpandList = true;
            Vector3 tempIndex;
            switch (i)
            {
            case 0:
                tempIndex = Vector3(roomIndexList[m_iRoomID].x - 1, 0, roomIndexList[m_iRoomID].z);
                if (CompareOverlap(tempIndex))
                    AddToExpandList = false;
                break;
            case 1:
                tempIndex = Vector3(roomIndexList[m_iRoomID].x + 1, 0, roomIndexList[m_iRoomID].z);
                if (CompareOverlap(tempIndex))
                    AddToExpandList = false;
                break;
            case 2:
                tempIndex = Vector3(roomIndexList[m_iRoomID].x, 0, roomIndexList[m_iRoomID].z - 1);
                if (CompareOverlap(tempIndex))
                    AddToExpandList = false;
                break;
            case 3:
                tempIndex = Vector3(roomIndexList[m_iRoomID].x, 0, roomIndexList[m_iRoomID].z + 1);
                if (CompareOverlap(tempIndex))
                    AddToExpandList = false;
                break;
            }

            // Add to expand list
            if (AddToExpandList)
            {
                // Give unique ID for expanded room
                tempRoomID++;
                // Room Mapping
                roomMap[roomID][i] = tempRoomID;

                roomIndexList.push_back(Vector3(tempIndex.x, 0, tempIndex.z));
                // Add index and room ID for creation
                expandList.push(Vector3(tempIndex.x, tempRoomID, tempIndex.z));
            }
        }
    }

    cout << "MAP: " << m_iRoomID << " " << roomMap[roomID][0] << " " <<
        roomMap[roomID][1] << " " <<
        roomMap[roomID][2] << " " <<
        roomMap[roomID][3] << endl;

    cout << "expand list: " << expandList.size() << endl;
    roomList.push_back(room);
    m_iRoomID++;
}

void CLevel::ExpandRoom(void)
{
    // Create room bound for expansion
    while (expandList.size() > 0)
    {
        Vector3 temp = expandList.front();

        CreateRoom(temp.y, ROOMSIZE, ROOMSIZE, temp.x, temp.z);

        expandList.pop();
    }
}

void CLevel::AddSpecialRoom(const int xSize, const int zSize,
                            ROOM_TYPE _type)
{
    // Create room mapping
    roomMap[m_iRoomID];

    for (int roomIter = roomList.size(); roomIter >= 0; --roomIter)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (roomMap[roomIter][i] >= 0)
                continue;

            // Set Index for room mapping & Check for overlap
            bool AddSpecialRoom = true;
            Vector3 tempIndex;
            switch (i)
            {
            case 0:
                tempIndex = Vector3(roomIndexList[roomIter].x - 1, 0, roomIndexList[roomIter].z);
                if (CompareOverlap(tempIndex))
                    AddSpecialRoom = false;
                break;
            case 1:
                tempIndex = Vector3(roomIndexList[roomIter].x + 1, 0, roomIndexList[roomIter].z);
                if (CompareOverlap(tempIndex))
                    AddSpecialRoom = false;
                break;
            case 2:
                tempIndex = Vector3(roomIndexList[roomIter].x, 0, roomIndexList[roomIter].z - 1);
                if (CompareOverlap(tempIndex))
                    AddSpecialRoom = false;
                break;
            case 3:
                tempIndex = Vector3(roomIndexList[roomIter].x, 0, roomIndexList[roomIter].z + 1);
                if (CompareOverlap(tempIndex))
                    AddSpecialRoom = false;
                break;
            }

            if (AddSpecialRoom)
            {
                // Create and add the room
                CRoom* room = new CRoom();
                room->Add(m_iRoomID, xSize, zSize, tempIndex.x, tempIndex.z, _type);

                roomMap[roomIter][i] = m_iRoomID;

                roomIndexList.push_back(Vector3(tempIndex.x, 0, tempIndex.z));

                roomList.push_back(room);
                m_iRoomID++;

                cout << "SPECIAL ROOM MAP: " << m_iRoomID << " " <<
                    roomMap[roomIter][0] << " " <<
                    roomMap[roomIter][1] << " " <<
                    roomMap[roomIter][2] << " " <<
                    roomMap[roomIter][3] << endl;

                return;
            }
        }
    }
}

void CLevel::SetDoor(CRoom* room)
{
    int xMin = room->GetRoomXMin();
    int xMax = room->GetRoomXMax();
    int zMin = room->GetRoomZMin();
    int zMax = room->GetRoomZMax();

    int tempRoomID = room->GetRoomID();
    Vector3 tempRoomIndex = room->GetIndex();

    for (int x = xMin; x <= xMax; ++x)
    {
        for (int z = zMin; z <= zMax; ++z)
        {
            // Set next room ID
            // Left
            if (x == xMin && z == (zMax >> 1) && CLevel::GetInstance()->GetRoom(tempRoomIndex.x - 1, tempRoomIndex.z) != -1)
            {
                room->GetSpatialPartition()->SetGridType(x, z, GRID_TYPE::DOOR);
                room->GetSpatialPartition()->SetGridType(x + 1, z, GRID_TYPE::DOORPATH);
                room->SetDoorToRoomID(0, CLevel::GetInstance()->GetRoom(tempRoomIndex.x - 1, tempRoomIndex.z));
            }
            // Right
            if (x == xMax && z == (zMax >> 1) && CLevel::GetInstance()->GetRoom(tempRoomIndex.x + 1, tempRoomIndex.z) != -1)
            {
                room->GetSpatialPartition()->SetGridType(x, z, GRID_TYPE::DOOR);
                room->GetSpatialPartition()->SetGridType(x - 1, z, GRID_TYPE::DOORPATH);
                room->SetDoorToRoomID(1, CLevel::GetInstance()->GetRoom(tempRoomIndex.x + 1, tempRoomIndex.z));
            }
            // Up
            if (z == zMin && x == (xMax >> 1) && CLevel::GetInstance()->GetRoom(tempRoomIndex.x, tempRoomIndex.z - 1) != -1)
            {
                room->GetSpatialPartition()->SetGridType(x, z, GRID_TYPE::DOOR);
                room->GetSpatialPartition()->SetGridType(x, z + 1, GRID_TYPE::DOORPATH);
                room->SetDoorToRoomID(2, CLevel::GetInstance()->GetRoom(tempRoomIndex.x, tempRoomIndex.z - 1));
            }
            // Down
            if (z == zMax && x == (xMax >> 1) && CLevel::GetInstance()->GetRoom(tempRoomIndex.x, tempRoomIndex.z + 1) != -1)
            {
                room->GetSpatialPartition()->SetGridType(x, z, GRID_TYPE::DOOR);
                room->GetSpatialPartition()->SetGridType(x, z - 1, GRID_TYPE::DOORPATH);
                room->SetDoorToRoomID(3, CLevel::GetInstance()->GetRoom(tempRoomIndex.x, tempRoomIndex.z + 1));
            }
        }
    }

    cout << "SIZE: " << xMin << " " << xMax << " " << zMin << " " << zMax << " ";
    cout << "ROOM ID: " << tempRoomID << " " << CLevel::GetInstance()->GetRoom(tempRoomIndex.x - 1, tempRoomIndex.z) << " " << CLevel::GetInstance()->GetRoom(tempRoomIndex.x + 1, tempRoomIndex.z) << " " <<
        CLevel::GetInstance()->GetRoom(tempRoomIndex.x, tempRoomIndex.z - 1) << " " << CLevel::GetInstance()->GetRoom(tempRoomIndex.x, tempRoomIndex.z + 1) << endl;
}