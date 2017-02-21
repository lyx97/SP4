#pragma once

#include "SingletonTemplate.h"
#include <vector>
#include <queue>
#include <map>
#include "Vector3.h"
#include "../EntityManager.h"
#include "Room.h"

static const int ROOMSIZE = 19;

class CLevel : public Singleton<CLevel>
{
    friend Singleton<CLevel>;
protected:

    int m_iRoomID;

    float m_fRoomBias;

    std::vector<Vector3> roomIndexList;
    std::vector<CRoom*> roomList;

    std::queue<Vector3> expandList;

    std::map<int, int[4]> roomMap;

public:
    CLevel();
    virtual ~CLevel();

    void Init(const float room_bias);

    CRoom* GetRoom(const int roomID);
    int GetRoom(const int xIndex, const int zIndex);
    
    const bool CompareOverlap(Vector3 index);

    void CreateRoom(const int roomID,
                    const int xSize, const int zSize,
                    const int xIndex, const int zIndex,
                    bool firstRoom = false);

    void ExpandRoom(void);

    void SetDoor(CRoom* room);

    void CleanRoomList(void);

    inline int GetNumRoom(void) { return roomList.size(); }
    inline std::vector<Vector3> GetRoomIndex(void) { return roomIndexList; }
};