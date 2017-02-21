#pragma once

#include <map>
#include <Queue>
#include "SingletonTemplate.h"
#include "Vector3.h"
#include "Heatmap.h"

class CGenerateHeatmap : public Singleton<CGenerateHeatmap>
{
    friend Singleton<CGenerateHeatmap>;
private:
    int count;

    std::queue<Vector3> child;

    CGenerateHeatmap();
    ~CGenerateHeatmap();

public:
    // Generate Heatmap 
    void GenerateHeatmap(CHeatmap** heatmap, 
                        const int xSize, const int zSize,
                        const int xIndex, const int zIndex);
    // Brushfire start
    void StartBrushfire(CHeatmap** heatmap,
                        const int xSize, const int zSize, 
                        const int xIndex, const int zIndex, 
                        const int prevValue);
    // Add child fire
    void AddChild(const int xIndex, const int zIndex, int prevValue);
    // Start child fire if child list not empty
    void ChildBrushfire(CHeatmap** heatmap, const int xSize, const int zSize);

    // Calculate direction vector to goal
    void CalculateDirection(CHeatmap** heatmap, const int xSize, const int zSize);

};