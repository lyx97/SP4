#include "GenerateHeatmap.h"
#include "SpatialPartition.h"
#include "Level.h"

CGenerateHeatmap::CGenerateHeatmap()
    : count(0)
{

}

CGenerateHeatmap::~CGenerateHeatmap()
{

}

// Generate Heatmap
void CGenerateHeatmap::GenerateHeatmap(CHeatmap** heatmap, 
    const int xSize, const int zSize,
    const int xIndex, const int zIndex)
{
    // Init
    for (int x = 0; x <= xSize; ++x)
    {
        for (int z = 0; z <= zSize; ++z)
        {
            heatmap[x][z].SetPos(Vector3(x, 0, z));
            heatmap[x][z].SetValue(0);
            if (heatmap[x][z].isMarked())
                heatmap[x][z].SetMarked(false);
            if (heatmap[x][z].isObstacle())
                heatmap[x][z].SetObstacle(false);
        }
    }

    heatmap[xIndex][zIndex].SetValue(0);
    heatmap[xIndex][zIndex].SetMarked(true);

    // Generate
    StartBrushfire(heatmap, xSize, zSize, xIndex, zIndex, 0);
    ChildBrushfire(heatmap, xSize, zSize);
}

// Brushfire start
void CGenerateHeatmap::StartBrushfire(CHeatmap** heatmap,
    const int xSize, const int zSize, 
    const int xIndex, const int zIndex, 
    const int prevValue)
{
    Vector3 ChildA = Vector3(xIndex, 0, zIndex) + Vector3(-1, 0, 0);
    Vector3 ChildB = Vector3(xIndex, 0, zIndex) + Vector3(1, 0, 0);
    Vector3 ChildC = Vector3(xIndex, 0, zIndex) + Vector3(0, 0, -1);
    Vector3 ChildD = Vector3(xIndex, 0, zIndex) + Vector3(0, 0, 1);

    for (int i = 0; i < 4; ++i)
    {
        Vector3 temp;
        if (i == 0)
            temp = ChildA;
        else if (i == 1)
            temp = ChildB;
        else if (i == 2)
            temp = ChildC;
        else if (i == 3)
            temp = ChildD;

        if (0 <= temp.x && temp.x <= xSize &&
            0 <= temp.z && temp.z <= zSize)
        {
            int x = temp.x;
            int z = temp.z;

            if (!heatmap[x][z].isMarked())
            {
                count++;
                if (CLevel::GetInstance()->GetRoom(CPlayerInfo::GetInstance()->GetRoomID())->GetSpatialPartition()->GetGridType(x, z)
                    != GRID_TYPE::OBSTACLE)
                {
                    heatmap[x][z].SetValue(heatmap[x][z].GetValue() + prevValue + 1);
                    heatmap[x][z].SetMarked(true);
                    AddChild(x, z, prevValue + 1);
                }
                else
                {
                    heatmap[x][z].SetValue(99999);
                    heatmap[x][z].SetMarked(true);
                    heatmap[x][z].SetObstacle(true);

                    //heatmap[x - 1][z - 1].SetValue(heatmap[x - 1][z - 1].GetValue() - 2);
                    //heatmap[x + 1][z - 1].SetValue(heatmap[x + 1][z - 1].GetValue() - 2);
                    //heatmap[x - 1][z + 1].SetValue(heatmap[x - 1][z + 1].GetValue() - 2);
                    //heatmap[x + 1][z + 1].SetValue(heatmap[x + 1][z + 1].GetValue() - 2);
                }
            }

        }
    }

}

// Add child fire
void CGenerateHeatmap::AddChild(const int xIndex, const int zIndex, int prevValue)
{
    child.push(Vector3(xIndex, 0, zIndex));
    child.push(Vector3(prevValue, 0, 0));
}

// Start child fire if child list not empty
void CGenerateHeatmap::ChildBrushfire(CHeatmap** heatmap, const int xSize, const int zSize)
{
    while (!child.empty())
    {
        Vector3 temp = child.front();
        child.pop();
        int tempValue = child.front().x;
        child.pop();

        StartBrushfire(heatmap, xSize, zSize, temp.x, temp.z, tempValue);
    }
}

// Calculate direction vector to goal
void CGenerateHeatmap::CalculateDirection(CHeatmap** heatmap, const int xSize, const int zSize)
{
    for (int x = 0; x <= xSize; ++x)
    {
        for (int z = 0; z <= zSize; ++z)
        {
            //if (!heatmap[x][z].isObstacle())
            {
                // Check if up/down/left/right of grid has obstacle
                bool allowDiagonalPath = true;
                for (int i = -1; i <= 1; ++i)
                {
                    for (int j = -1; j <= 1; ++j)
                    {
                        if (i == j || abs(i) == j || i == abs(j))
                            continue;

                        if (i == 0 && j == 0)
                            continue;

                        if (x + i >= 0 && x + i <= xSize &&
                            z + j >= 0 && z + j <= zSize)
                        {
                            if (heatmap[x + i][z + j].isObstacle())
                                allowDiagonalPath = false;
                        }
                    }
                }

                int lowest = 99999;
                if (allowDiagonalPath)
                {
                    for (int i = -1; i <= 1; ++i)
                    {
                        for (int j = -1; j <= 1; ++j)
                        {
                            if (x + i >= 0 && x + i <= xSize &&
                                z + j >= 0 && z + j <= zSize)
                            {
                                int temp = heatmap[x + i][z + j].GetValue();
                                if (lowest > temp)
                                {
                                    lowest = temp;
                                    heatmap[x][z].SetDir((heatmap[x + i][z + j].GetPos() - heatmap[x][z].GetPos()));
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (int i = -1; i <= 1; ++i)
                    {
                        for (int j = -1; j <= 1; ++j)
                        {
                            if (i == j || abs(i) == j || i == abs(j))
                                continue;

                            if (i == 0 && j == 0)
                                continue;

                            if (x + i >= 0 && x + i <= xSize &&
                                z + j >= 0 && z + j <= zSize)
                            {
                                int temp = heatmap[x + i][z + j].GetValue();
                                if (lowest > temp)
                                {
                                    lowest = temp;
                                    heatmap[x][z].SetDir((heatmap[x + i][z + j].GetPos() - heatmap[x][z].GetPos()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}