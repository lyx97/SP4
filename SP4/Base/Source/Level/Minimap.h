#pragma once

#include "Vector3.h"
#include "EntityBase.h"
#include "Mesh.h"

class Application;

class CMinimap : public EntityBase
{
protected:
    Vector3 position;
    Vector3 scale;

    Mesh* Border;
    Mesh* Background;
    Mesh* Rooms;
    Mesh* Player;

public:
    CMinimap();
    virtual ~CMinimap();

    void Init(void);

    void Update(double dt = 0.0333f);
    void RenderUI(void);

};