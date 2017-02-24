#include "Room.h"
#include "../EntityManager.h"
#include "../Items/Powerup.h"

CRoom::CRoom()
    : m_iRoomID(-1)
    , m_iMin_X(-1)
    , m_iMax_X(-1)
    , m_iMin_Z(-1)
    , m_iMax_Z(-1)
    , m_iPreviousID(-1)
    , m_bCleared(true)
    , index(Vector3(-1, -1, -1))
    , roomSpatial(NULL)
{
    for (int i = 0; i < 4; ++i)
        m_iStoredID[i] = -1;
}

CRoom::~CRoom()
{

}

void CRoom::Spawn(void)
{
    switch (m_eRoomType)
    {
    case ROOM_TYPE::STARTROOM:

        break;
    case ROOM_TYPE::ENEMYROOM:

        break;
    case ROOM_TYPE::SHOPROOM:

        break;
    case ROOM_TYPE::PUZZLEROOM:

        break;
    case ROOM_TYPE::TREASUREROOM:
        break;
    }
}

void CRoom::Add(const int& roomID,
                const int& xNum, const int& zNum,
                const int& xIndex, const int& zIndex,
                bool firstRoom)
{
    // Create a new spatial partition for the room
    roomSpatial = new CSpatialPartition();
    roomSpatial->Init(GRIDSIZE, GRIDSIZE, xNum, zNum, 0, 0);
    EntityManager::GetInstance()->SetSpatialPartition(roomSpatial);

    // Set room type
    if (firstRoom)
        m_eRoomType = ROOM_TYPE::STARTROOM;
    else
    {
        int tempRand = Math::RandIntMinMax(1, NUM_ROOMTYPE - 1);
        switch (tempRand)
        {
        case 1:
            m_eRoomType = ROOM_TYPE::ENEMYROOM;
            break;
        case 2:
            m_eRoomType = ROOM_TYPE::SHOPROOM;
            break;
        case 3:
            m_eRoomType = ROOM_TYPE::PUZZLEROOM;
            break;
        case 4:
            m_eRoomType = ROOM_TYPE::TREASUREROOM;
            break;
        }
    }

    // Spawn entities based on room
    Spawn();

    // Set room ID
    m_iRoomID = roomID;

    // Set number of grids
    m_iMin_X = 0;
    m_iMax_X = xNum - 1;
    m_iMin_Z = 0;
    m_iMax_Z = zNum - 1;

    // Set room index
    index = Vector3(xIndex, 0, zIndex);

    for (int x = m_iMin_X; x <= m_iMax_X; ++x)
    {
        for (int z = m_iMin_Z; z <= m_iMax_Z; ++z)
        {
            if (x == m_iMin_X && z <= m_iMax_Z ||
                x == m_iMax_X && z >= m_iMin_Z ||
                z == m_iMin_Z && x <= m_iMax_X ||
                z == m_iMax_Z && x >= m_iMin_X)
                roomSpatial->SetGridType(x, z, GRID_TYPE::WALL);
            else
                roomSpatial->SetGridType(x, z, GRID_TYPE::PATH);
        }
    }

    cout << "Room Created: " << m_iRoomID << endl;
}
