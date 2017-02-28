#pragma once

#include "../GenericEntity.h"

enum OBSTACLE_TYPE
{
    BLOCK,
    TRAP,
    NUM_OBSTACLE
};

class Obstacle : public GenericEntity
{
private:

public:
    Obstacle(const Vector3 _pos, const int _roomID);
    virtual ~Obstacle();

    virtual void Update(double _dt);
    virtual void Render(float& _renderOrder);
};