#pragma once

#include "Vector3.h"

class CHeatmap
{
protected:
    int value;

    bool marked;
    bool obstacle;

    Vector3 Pos;
    Vector3 Dir;

public:
    CHeatmap();
    ~CHeatmap();

    int GetValue(void) const;
    void SetValue(int value);

    bool isMarked(void) const;
    void SetMarked(bool marked);

    bool isObstacle(void) const;
    void SetObstacle(bool obstacle);

    Vector3 GetDir(void);
    void SetDir(Vector3 dir);

    Vector3 GetPos(void);
    void SetPos(Vector3 pos);
};