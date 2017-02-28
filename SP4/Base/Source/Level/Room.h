#pragma once

#include "Vector3.h"
#include "SpatialPartition.h"

static const int GRIDSIZE = 20;

class EntityManager;
class Powerup;

enum ROOM_TYPE
{
    STARTROOM = 0,
    ENEMYROOM,
    SHOPROOM,
    PUZZLEROOM,
    TREASUREROOM,
    NUM_ROOMTYPE
};

class CRoom
{
protected:
    ROOM_TYPE m_eRoomType;

    int m_iRoomID;
    int m_iMin_X;
    int m_iMax_X;
    int m_iMin_Z;
    int m_iMax_Z;
     
    // Store for previous room ID
    int m_iPreviousID;
    // Store for connected room ID
    int m_iStoredID[4];

    bool m_bCleared;

    Vector3 index;

    CSpatialPartition* RoomSpatial;

public:
    CRoom();
    ~CRoom();

    void Add(const int& roomID,
             const int& xNum, const int& zNum,
             const int& xIndex, const int& zIndex,
             bool firstRoom = false);

    // Spawn entities for each room generated
    void Spawn(void);

    inline const ROOM_TYPE GetRoomType(void) { return m_eRoomType; }
    inline void SetRoomType(ROOM_TYPE roomType) { m_eRoomType = roomType; }

    inline const int GetRoomID(void) { return m_iRoomID; }
    inline void SetRoomID(const int roomID) { m_iRoomID = roomID; }

    inline const int GetRoomXMin(void) { return m_iMin_X; }
    inline const int GetRoomXMax(void) { return m_iMax_X; }
    inline const int GetRoomZMin(void) { return m_iMin_Z; }
    inline const int GetRoomZMax(void) { return m_iMax_Z; }
    
    inline const int GetDoorToRoomID(const int id) { return m_iStoredID[id]; }
    inline void SetDoorToRoomID(const int id, const int value) { m_iStoredID[id] = value; }

    inline const int GetPreviousRoomID(void) { return m_iPreviousID; }
    inline void SetPreviousRoomID(const int id) { m_iPreviousID = id; }

    inline const bool GetRoomCleared(void) { return m_bCleared; }
    inline void SetRoomCleared(const bool cleared) { m_bCleared = cleared; }

    inline Vector3 GetIndex(void) { return index; }
    inline void SetIndex(const int xIndex, const int zIndex) { index = Vector3(xIndex, 0, zIndex); }

    inline CSpatialPartition* GetSpatialPartition() { return RoomSpatial; }
};