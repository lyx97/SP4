#include "Heatmap.h"

CHeatmap::CHeatmap()
    : value(0)
    , marked(false)
    , obstacle(false)
    , Dir(Vector3(0,0,0))
{

}

CHeatmap::~CHeatmap()
{

}


int CHeatmap::GetValue(void) const
{
    return value;
}

void CHeatmap::SetValue(int value)
{
    this->value = value;
}

bool CHeatmap::isMarked(void) const
{
    return marked;
}

void CHeatmap::SetMarked(bool marked)
{
    this->marked = marked;
}

bool CHeatmap::isObstacle(void) const
{
    return obstacle;
}

void CHeatmap::SetObstacle(bool obstacle)
{
    this->obstacle = obstacle;
}

Vector3 CHeatmap::GetDir(void)
{
    return Dir;
}

void CHeatmap::SetDir(Vector3 dir)
{
    Dir = dir;
}

Vector3 CHeatmap::GetPos(void)
{
    return Pos;
}

void CHeatmap::SetPos(Vector3 pos)
{
    Pos = pos;
}