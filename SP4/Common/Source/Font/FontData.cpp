#include "FontData.h"
#include <iostream>

FontData::FontData()
{
	for (unsigned i = 0; i < charTotal; ++i)
	{
		baseWidth[i] = 0;
		widthOffset[i] = 0;
	}
}

FontData::~FontData()
{
}

void FontData::LoadBaseWidth(const std::vector<int> &data)
{
	if (data.size() != charTotal) return;
    for (unsigned i = 0; i < charTotal; ++i)
    {
        baseWidth[i] = data[i];
        //std::cout << "";
    }
}

void FontData::LoadWidthOffset(const std::vector<int> &data)
{
	if (data.size() != charTotal) return;
	for (unsigned i = 0; i < charTotal; ++i)
		widthOffset[i] = data[i];
}

void FontData::SetCellWidth(const int &w)
{
	cellWidth = w;
}

int FontData::GetBaseWidth(const unsigned &index) const
{
	if (index >= charTotal) return 0;
	return baseWidth[index];
}

int FontData::GetWidthOffset(const unsigned &index) const
{
	if (index >= charTotal) return 0;
	return widthOffset[index];
}

float FontData::GetBWRatio(const unsigned &index) const
{
	if (index >= charTotal) return 0.f;
	return (float)baseWidth[index] / (float)cellWidth;
}

float FontData::GetWORatio(const unsigned &index) const
{
	if (index >= charTotal) return 0.f;
	return (float)widthOffset[index] / (float)cellWidth;
}