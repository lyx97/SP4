#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include <queue>
#include "Vector3.h"
#include "Level/SpatialPartition.h"
#include "Enemy\Enemy2D.h"

class EntityBase;
class CPlayerInfo;
class Particle;
class Enemy2D;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
    void RenderUI();

    void AddEntity(EntityBase* _newEntity, const int _roomID = -1);
    bool RemoveEntity(EntityBase* _existingEntity, const int _roomID = -1);
    bool MarkForDeletion(EntityBase* _existingEntity);

    void AddParticleActive(void);
    void AddParticleInactive(void);
    void RemoveParticleActive(Particle* _Particle);
    Particle* GetParticle(void);

    void ResetEntities(void);

	std::list<EntityBase*> GetEntityList() { return entityList; };

    CSpatialPartition* GetSpatialPartition(const int roomID);
    void SetSpatialPartition(CSpatialPartition* theSpatialPartition);

    inline void RemoveSpatialPartition(void) { partitionList.clear(); }

    static std::list<Vector3> tilePosList;
    static std::list<Vector3> particlePosList;

private:
	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
    // Check for intersection between a line segment and a plane
    bool GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
    // Check for intersection between a line segment and a plane
    bool CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end,
        Vector3 minAABB, Vector3 maxAABB,
        Vector3 &Hit);
    // Check two positions are within a box region
    bool InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);
	// Check if any Collider is colliding with another Collider
	void CheckForCollision(void);

	std::list<EntityBase*> entityList;
    std::list<Particle*> particleList_Active;
    std::list<Particle*> particleList_Inactive;

    std::queue<std::list<Particle*>::iterator> SpliceQueue;

    void Splice(std::list<Particle*>::iterator _particle, bool _IsDone);

    // Handler to Spatial Partition
    std::vector<CSpatialPartition*> partitionList;
};

#endif // ENTITY_MANAGER_H