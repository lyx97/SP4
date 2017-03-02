#pragma once

#include "SingletonTemplate.h"
#include <vector>
#include <queue>
#include <map>
#include "Vector3.h"
#include "../EntityManager.h"
#include "Room.h"

static const int ROOMSIZE = 19;

class CPlayerInfo;

class CLevel : public Singleton<CLevel>
{
private:
    friend Singleton<CLevel>;

    CLevel();
    virtual ~CLevel();

protected:
    int m_iLevel = 1;
    int m_iRoomID;

    float m_fRoomBias;

    std::vector<Vector3> roomIndexList;
    std::vector<int> roomNumEnemyList;
    std::vector<CRoom*> roomList;

    std::queue<Vector3> expandList;

    std::map<int, int[4]> roomMap;

public:
    void Init(const float room_bias);

    void Render();

    CRoom* GetRoom(const int roomID);
    int GetRoom(const int xIndex, const int zIndex);
    
    const bool CompareOverlap(Vector3 index);

    void CreateRoom(const int roomID,
                    const int xSize, const int zSize,
                    const int xIndex, const int zIndex,
                    bool firstRoom = false);

    void ExpandRoom(void);

    void AddSpecialRoom(const int xSize, const int zSize,
                        ROOM_TYPE _type);

    // Link room
    void SetDoor(CRoom* room);

    inline int GetNumRoom(void) { return roomList.size(); }
    inline std::vector<Vector3> GetRoomIndex(void) { return roomIndexList; }

    inline int GetLevel(void) { return m_iLevel; }
    inline void IncreaseLevel(void) { m_iLevel += 1; }

    inline void AddNumEnemyToList(const int _value) { roomNumEnemyList.push_back(_value); }
    inline void DecreaseEnemyInList(const int _roomID) { roomNumEnemyList[_roomID]--; }
    inline int GetNumEnemyFromList(const int _roomID) { return roomNumEnemyList[_roomID]; }
};