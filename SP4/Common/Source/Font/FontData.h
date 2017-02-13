#pragma once
#include <vector>

/******************************************************************************/
/*!
Class FontData:
\brief
Defines the FontData and its methods
*/
/******************************************************************************/

class FontData
{
public:
	FontData();
	~FontData();

	void LoadBaseWidth(const std::vector<int> &data);
	void LoadWidthOffset(const std::vector<int> &data);
	void SetCellWidth(const int &w);
	int GetBaseWidth(const unsigned &index) const;
	int GetWidthOffset(const unsigned &index) const;
	float GetBWRatio(const unsigned &index) const;
	float GetWORatio(const unsigned &index) const;

	static const size_t charTotal = 256;
private:
	int cellWidth;
	int baseWidth[charTotal];
	int widthOffset[charTotal];
};