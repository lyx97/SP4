#include "Room.h"
#include "../EntityManager.h"
#include "../Items/Powerup.h"
#include "../Enemy/FlyingTongue.h"
#include "../Enemy/Skull.h"
#include "../Enemy/Spitter.h"
#include "../Enemy/Reaper.h"
#include "../Obstacle/Obstacle.h"
#include "Level.h"

CRoom::CRoom()
    : m_iRoomID(-1)
    , m_iMin_X(-1)
    , m_iMax_X(-1)
    , m_iMin_Z(-1)
    , m_iMax_Z(-1)
    , m_iPreviousID(-1)
    , m_bCleared(false)
    , index(Vector3(-1, -1, -1))
    , RoomSpatial(NULL)
{
    for (int i = 0; i < 4; ++i)
        m_iStoredID[i] = -1;
}

CRoom::~CRoom()
{

}

void CRoom::Add(const int& roomID,
                const int& xNum, const int& zNum,
                const int& xIndex, const int& zIndex,
                ROOM_TYPE _type)
{
    // Create a new spatial partition for the room
    RoomSpatial = new CSpatialPartition();
    RoomSpatial->Init(GRIDSIZE, GRIDSIZE, xNum, zNum, 0, 0);
    EntityManager::GetInstance()->SetSpatialPartition(RoomSpatial);

    // Set room type
    m_eRoomType = _type;

    // Set if room cleared
    m_bCleared = false;

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
                RoomSpatial->SetGridType(x, z, GRID_TYPE::WALL);
            else
                RoomSpatial->SetGridType(x, z, GRID_TYPE::PATH);
        }
    }

    // Spawn entities based on room
    Spawn();

    cout << "Room Created: " << m_iRoomID << endl;
}

void CRoom::Spawn(void)
{
    list<Vector3> OccupiedGridList;

    switch (m_eRoomType)
    {
    case ROOM_TYPE::STARTROOM:
    {
        //float spawnbias = 10.f;

        //for (int x = m_iMin_X + 1; x <= m_iMax_X - 1; ++x)
        //{
        //    for (int z = m_iMin_Z + 1; z <= m_iMax_Z - 1; ++z)
        //    {
        //        if (spawnbias >= Math::RandFloatMinMax(0, 100))
        //        {
        //            spawnbias *= 0.75f;

        //            Obstacle* obstacle = new Obstacle(RoomSpatial->GetGridPos(x, z), m_iRoomID);
        //            RoomSpatial->GetGrid(x, z)->SetType(GRID_TYPE::OBSTACLE);
        //        }
        //    }
        //}

        CLevel::GetInstance()->AddNumEnemyToList(0);
        break;
    }
    case ROOM_TYPE::ENEMYROOM:
    {
        int enemyCount = 0;
        while (enemyCount < ENEMYSPAWNCOUNT)
        {
            Vector3 tempPos = Vector3(
                Math::RandIntMinMax(m_iMin_X + 1, m_iMax_X - 1),
                0,
                Math::RandIntMinMax(m_iMin_Z + 1, m_iMax_Z - 1));

            bool spawn = true;

            for (auto it : OccupiedGridList)
            {
                if (it == tempPos)
                    spawn = false;
                    break;
            }

            if (spawn)
            {
                int randspawn = Math::RandIntMinMax(0, 2);

                switch (randspawn)
                {
                case 0:
                {                    
                    Spitter* spitter = new Spitter(m_iRoomID);
                    spitter->SetPosition(RoomSpatial->GetGridPos(tempPos.x, tempPos.z));
                    break;
                }
                case 1:
                {
                    FlyingTongue* flyingtongue = new FlyingTongue(m_iRoomID);
                    flyingtongue->SetPosition(RoomSpatial->GetGridPos(tempPos.x, tempPos.z));
                    break;
                }
                case 2:
                {
                    Skull* skull = new Skull(m_iRoomID);
                    skull->SetPosition(RoomSpatial->GetGridPos(tempPos.x, tempPos.z));
                    break;
                }
                }
                OccupiedGridList.push_back(Vector3(tempPos.x, 0, tempPos.z));
                enemyCount++;
            }
        }

        int obstacleCount = Math::RandIntMinMax(15, 20);
        while (obstacleCount != 0)
        {
            Vector3 tempPos = Vector3(
                Math::RandIntMinMax(m_iMin_X + 2, m_iMax_X - 2),
                0,
                Math::RandIntMinMax(m_iMin_Z + 2, m_iMax_Z - 2));

            bool spawn = true;

            for (auto it : OccupiedGridList)
            {
                if (it == tempPos)
                    spawn = false;
                break;
            }

            if (spawn)
            {
                Obstacle* obstacle = new Obstacle(RoomSpatial->GetGridPos(tempPos.x, tempPos.z), m_iRoomID);
                RoomSpatial->GetGrid(tempPos.x, tempPos.z)->SetType(GRID_TYPE::OBSTACLE);
                OccupiedGridList.push_back(Vector3(tempPos.x, 0, tempPos.z));
                obstacleCount--;
            }
        }

        CLevel::GetInstance()->AddNumEnemyToList(ENEMYSPAWNCOUNT);
        break;
    }
    case ROOM_TYPE::TREASUREROOM:
    {
        Treasure* newTreasure = new Treasure(m_iRoomID);
        CLevel::GetInstance()->AddNumEnemyToList(0);
        break;
    }
    case ROOM_TYPE::NEXTLEVELROOM:
    {
        if (CLevel::GetInstance()->GetLevel() == 3)
        {
            Reaper* reaper = new Reaper(m_iRoomID);
            CLevel::GetInstance()->AddNumEnemyToList(1);
        }
        else
        {
            RoomSpatial->GetGrid(m_iMax_X >> 1, m_iMax_Z >> 1)->SetType(GRID_TYPE::STAIR);
            CLevel::GetInstance()->AddNumEnemyToList(0);
        }
        break;
    }
    }
}