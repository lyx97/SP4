#pragma once

#include "Vector3.h"
#include "EntityBase.h"
#include "Mesh.h"

const int BGSCALE = 150;
const int ROOMSCALE = 15;
const int PLAYERSCALE = 10;

class Application;
class CPlayerInfo;
class CLevel;

class CMinimap : public EntityBase
{
protected:
    Mesh* Border;
    Mesh* Background;
    Mesh* Path;
    Mesh* Room;
    Mesh* Player;

    Vector3 position;

public:
    CMinimap();
    virtual ~CMinimap();

    void Update(double dt = 0.0333f);
    void RenderUI(void);

};