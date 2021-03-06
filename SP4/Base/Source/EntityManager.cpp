#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "../Source/Projectile/Projectile.h"
#include "../Source/Projectile/Laser.h"
#include "Enemy/Enemy2D.h"
#include "PlayerInfo/PlayerInfo.h"
#include "Particle/Particle.h"
#include "../Level/Level.h"
#include "BatchRendering.h"
#include "LoadTGA.h"
#include "Sound\SoundManager.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
    // Update all entities
    std::list<EntityBase*>::iterator it, end;
    end = entityList.end();
    for (it = entityList.begin(); it != end; ++it)
    {
        if ((*it)->GetRoomID() == CPlayerInfo::GetInstance()->GetRoomID())
            (*it)->Update(_dt);
    }

    // Check for Collision amongst entities with collider properties
    CheckForCollision();

    // Clean up entities that are done
    it = entityList.begin();
    while (it != end)
    {
        if ((*it)->IsDone())
        {
            // Update entitylist of room
            if ((*it)->GetEntityType() == EntityBase::ENEMY)
            {
                CLevel::GetInstance()->DecreaseEnemyInList((*it)->GetRoomID());
                if (CLevel::GetInstance()->GetNumEnemyFromList((*it)->GetRoomID()) == 0)
                    CLevel::GetInstance()->GetRoom((*it)->GetRoomID())->SetRoomCleared();
            }
            // Delete if done
            partitionList[(*it)->GetRoomID()]->Remove(*it);
            delete *it;
            it = entityList.erase(it);
        }
        else
        {
            // Move on otherwise
            ++it;
        }
    }

    //Update the Spatial Partition
    if (partitionList.size() > 0)
        partitionList[CPlayerInfo::GetInstance()->GetRoomID()]->Update();

    // Update objects for batch rendering
    BatchRendering::GetInstance()->Update(_dt);
}

// Render all entities
void EntityManager::Render()
{
    // Render all entities
    std::list<EntityBase*>::iterator it, end;
    end = entityList.end();
    float _renderOrder = 0.f;
    for (it = entityList.begin(); it != end; ++it)
    {
        if ((*it)->GetRoomID() == CPlayerInfo::GetInstance()->GetRoomID())
        {
            _renderOrder += 0.01f;
            (*it)->Render(_renderOrder);
        }
    }

    // Render the Spatial Partition
    //if (partitionList.size() > 0)
    //	partitionList[CPlayerInfo::GetInstance()->GetRoomID()]->Render();

    // Batch rendering
    BatchRendering::GetInstance()->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
    // Render all entities UI
    std::list<EntityBase*>::iterator it, end;
    end = entityList.end();
    for (it = entityList.begin(); it != end; ++it)
    {
        (*it)->RenderUI();
    }
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, const int _roomID)
{
    entityList.push_back(_newEntity);

    // Add to the Spatial Partition
    if (_roomID != -1 && partitionList.size() > 0)
        partitionList[_roomID]->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity, const int _roomID)
{
    // Find the entity's iterator
    std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

    // Delete the entity if found
    if (findIter != entityList.end())
    {
        delete *findIter;
        findIter = entityList.erase(findIter);

        if (_roomID != -1)
            partitionList[_roomID]->Remove(_existingEntity);

        return true;
    }
    // Return false if not found
    return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
    // Find the entity's iterator
    std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

    // Delete the entity if found
    if (findIter != entityList.end())
    {
        (*findIter)->SetIsDone(true);
        return true;
    }
    // Return false if not found
    return false;
}

CSpatialPartition* EntityManager::GetSpatialPartition(const int roomID)
{
    return partitionList[roomID];
}

// Set a handle to the Spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
    partitionList.push_back(theSpatialPartition);
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
    if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
        ||
        ((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
    {
        return true;
    }

    if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
        ||
        ((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
    {
        return true;
    }

    if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
        &&
        ((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
    {
        return true;
    }

    if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
        &&
        ((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
    {
        return true;
    }

    return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
    // Get the colliders for the 2 entities
    CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
    CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

    // Get the minAABB and maxAABB for each entity
    Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
    Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
    Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
    Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

    // if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is
    // greater than the distance squared between the 2 reference points of the 2 entities
    // then it could mean that they are colliding with each other
    if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
        DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
    {
        return true;
    }

    return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
    // Get the colliders for the 2 entities
    CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
    CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

    // Get the minAABB and maxAABB for each entity
    Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
    Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
    Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
    Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

    // Check for overlap
    if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
        return true;

    // If AABB collision check fails, then we need to check the other corners of the bounding boxes to
    // do more collision checks with other points on each bounding box
    Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
    Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);

    // Check for overlap
    if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
        return true;

    return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
    if ((fDst1 * fDst2) >= 0.0f)
        return false;
    if (fDst1 == fDst2)
        return false;
    Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
    return true;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end,
    Vector3 minAABB, Vector3 maxAABB,
    Vector3 &Hit)
{
    if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x,
        line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
        || (GetIntersection(line_start.y - minAABB.y, line_end.y -
        minAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
        || (GetIntersection(line_start.z - minAABB.z, line_end.z -
        minAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
        || (GetIntersection(line_start.x - maxAABB.x, line_end.x -
        maxAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
        || (GetIntersection(line_start.y - maxAABB.y, line_end.y -
        maxAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
        || (GetIntersection(line_start.z - maxAABB.z, line_end.z -
        maxAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3)))
        return true;

    return false;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
    if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
    if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
    if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
    return false;
}

void EntityManager::CheckForCollision(void)
{
    // Check for Collision
    std::list<EntityBase*>::iterator This, ThisEnd;
    std::list<EntityBase*>::iterator Other, OtherEnd;

    ThisEnd = entityList.end();
    for (This = entityList.begin(); This != ThisEnd; ++This)
    {
        if ((*This)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
            continue;

        if ((*This)->IsDone())
            continue;

        // Check if this entity is a CLaser type
        if ((*This)->GetIsLaser())
        {
            // Dynamic cast it to a CLaser class
            CLaser* laser = dynamic_cast<CLaser*>(*This);

            // Check for collision with another collider class
            OtherEnd = entityList.end();
            for (Other = entityList.begin(); Other != OtherEnd; ++Other)
            {
                if (This == Other)
                    continue;

                if ((*Other)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                    continue;

                if ((*This)->IsDone())
                    continue;

                if ((*Other)->IsDone())
                    continue;

                if ((*Other)->GetEntityType() == EntityBase::ENEMY)
                {
                    if ((*Other)->GetHP() <= 0)
                        continue;

                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)))
                        {
                            (*This)->SetIsDone(true);
                            (*Other)->SetHPDamaged(laser->GetDamage());
                            (*Other)->SetVelocity((*Other)->GetVelocity() + ((*This)->GetVelocity() * (*This)->GetMass()));

                            BatchRendering::GetInstance()->GetParticle(
                                (*This)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*This)->GetColor(),
                                -(*This)->GetVelocity() * (*This)->GetMass(),
                                NUMPARTICLESPAWN);

                            BatchRendering::GetInstance()->GetParticle(
                                (*This)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*Other)->GetColor(),
                                (*This)->GetVelocity() * (*This)->GetMass(),
                                NUMPARTICLESPAWN);

							SoundManager::GetInstance()->PlaySoundEffect2D("Sound//hit_click.wav");
                        }
                    }
                }
                else if ((*Other)->GetEntityType() == EntityBase::OBSTACLE)
                {
                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)))
                        {
                            (*This)->SetIsDone(true);

                            BatchRendering::GetInstance()->GetParticle(
                                (*This)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*Other)->GetColor(),
                                (*This)->GetVelocity() * (*This)->GetMass(),
                                NUMPARTICLESPAWN);

                            BatchRendering::GetInstance()->GetParticle(
                                (*This)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*This)->GetColor(),
                                -(*This)->GetVelocity() * (*This)->GetMass(),
                                NUMPARTICLESPAWN);
                        }
                    }
                }
            }
        }
/********************************************************************************
PLAYER
********************************************************************************/
        else if ((*This)->GetEntityType() == EntityBase::PLAYER)
        {
            if ((*This)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                continue;

            // Check for collision with another collider class
            OtherEnd = entityList.end();
            for (Other = entityList.begin(); Other != OtherEnd; ++Other)
            {
                if (This == Other)
                    continue;

                if ((*Other)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                    continue;

                if ((*Other)->GetEntityType() == EntityBase::ENEMYPROJECTILE)
                {
                    CProjectile* enemyProjectile = dynamic_cast<CProjectile*>(*Other);
                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)) && !CPlayerInfo::GetInstance()->IsInvincible())
                        {
                            (*Other)->SetIsDone(true);
                            CPlayerInfo::GetInstance()->SetHealth(CPlayerInfo::GetInstance()->GetHealth() - enemyProjectile->GetDamage());
                            CPlayerInfo::GetInstance()->SetIsInvincible(true);
                            (*This)->SetVelocity((*This)->GetVelocity() + ((*Other)->GetVelocity()));

                            BatchRendering::GetInstance()->GetParticle(
                                (*Other)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*This)->GetColor(),
                                (*Other)->GetVelocity().Normalized() * (*Other)->GetMass(),
                                NUMPARTICLESPAWN);

                            BatchRendering::GetInstance()->GetParticle(
                                (*Other)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*Other)->GetColor(),
                                -(*Other)->GetVelocity().Normalized() * (*Other)->GetMass(),
                                NUMPARTICLESPAWN);


							SoundManager::GetInstance()->PlaySoundEffect2D("Sound//hit.wav");
                        }
                    }
                }
                else if ((*Other)->GetEntityType() == EntityBase::ENEMY)
                {
                    Enemy2D* enemy = dynamic_cast<Enemy2D*>(*Other);
                    if ((*Other)->GetHP() <= 0)
                        continue;

                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)) && !CPlayerInfo::GetInstance()->IsInvincible())
                        {
                            CPlayerInfo::GetInstance()->SetHealth(CPlayerInfo::GetInstance()->GetHealth() - enemy->GetDamage());
                            CPlayerInfo::GetInstance()->SetIsInvincible(true);
                            (*This)->SetVelocity((*This)->GetVelocity() + ((*This)->GetPosition() - (*Other)->GetPosition()));
                        }
                    }
                }
            }
        }
/********************************************************************************
OBSTACLE
********************************************************************************/
        else if ((*This)->GetEntityType() == EntityBase::OBSTACLE)
        {
            if ((*This)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                continue;

            // Check for collision with another collider class
            OtherEnd = entityList.end();
            for (Other = entityList.begin(); Other != OtherEnd; ++Other)
            {
                if (This == Other)
                    continue;

                if ((*Other)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                    continue;

                if ((*Other)->GetEntityType() == EntityBase::ENEMYPROJECTILE)
                {
                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)))
                        {
                            (*Other)->SetIsDone(true);

                            BatchRendering::GetInstance()->GetParticle(
                                (*Other)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*This)->GetColor(),
                                (*Other)->GetVelocity() * 0.1f,
                                NUMPARTICLESPAWN);

                            BatchRendering::GetInstance()->GetParticle(
                                (*Other)->GetPosition(),
                                Vector3(2.f, 2.f, 2.f),
                                (*Other)->GetColor(),
                                -(*Other)->GetVelocity() * 0.1f,
                                NUMPARTICLESPAWN);
                        }
                    }
                }
                else if ((*Other)->GetEntityType() == EntityBase::PLAYER)
                {
                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)))
                        {
                            if (!((*This)->GetPosition() - (*Other)->GetPosition()).IsZero())
                                (*Other)->SetPosition((*Other)->GetPosition() - ((*This)->GetPosition() - (*Other)->GetPosition()).Normalized());
                        }
                    }
                }
                else if ((*Other)->GetEntityType() == EntityBase::ENEMY)
                {
                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)))
                        {
                            if (!((*This)->GetPosition() - (*Other)->GetPosition()).IsZero())
                                (*Other)->SetPosition((*Other)->GetPosition() - ((*This)->GetPosition() - (*Other)->GetPosition()).Normalized());
                        }
                    }
                }
            }
        }
/********************************************************************************
ENEMY
********************************************************************************/
        else if ((*This)->GetEntityType() == EntityBase::ENEMY)
        {
            if ((*This)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                continue;

            if ((*This)->GetHP() <= 0)
                continue;

            // Check for collision with another collider class
            OtherEnd = entityList.end();
            for (Other = entityList.begin(); Other != OtherEnd; ++Other)
            {
                if (This == Other)
                    continue;

                if ((*Other)->GetRoomID() != CPlayerInfo::GetInstance()->GetRoomID())
                    continue;

                if ((*Other)->GetHP() <= 0)
                    continue;

                if ((*Other)->GetEntityType() == EntityBase::ENEMY)
                {
                    if ((*Other)->HasCollider())
                    {
                        if (CheckSphereCollision((*This), (*Other)))
                        {
                            if (!((*Other)->GetPosition() - (*This)->GetPosition()).IsZero())
                                (*This)->SetPosition((*This)->GetPosition() - ((*Other)->GetPosition() - (*This)->GetPosition()).Normalized());
                            if (!((*This)->GetPosition() - (*Other)->GetPosition()).IsZero())
                                (*Other)->SetPosition((*Other)->GetPosition() - ((*This)->GetPosition() - (*Other)->GetPosition()).Normalized());
                        }
                    }
                }
            }
        }
    }
}

void EntityManager::ResetEntities(void)
{
    std::list<EntityBase*>::iterator it, end;
    end = entityList.end();
    it = entityList.begin();
    while (it != end)
    {
        if ((*it)->GetEntityType() != EntityBase::PLAYER &&
            (*it)->GetEntityType() != EntityBase::DEFAULT)
        {
            // Delete if done
            partitionList[(*it)->GetRoomID()]->Remove(*it);
            delete *it;
            it = entityList.erase(it);
        }
        else
        {
            // Move on otherwise
            ++it;
        }
    }

}