#include "LoadFontData.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace::std;

FontData LoadFontData(const char *file_path)
{
	FontData fd;

	cout << "Loading \"" << file_path << "\"... ";

	ifstream fileStream(file_path, ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << " File not found.\n";
		return fd;
	}

	static unsigned lineNumber;
	lineNumber = 1;

		vector<int> data[2];
		data[0].resize(FontData::charTotal);
		data[1].resize(FontData::charTotal);

	while (fileStream.good())
	{
		std::string line;
		getline(fileStream, line);

		if (lineNumber == 3u)
		{
			string token;
			istringstream s(line);

			bool first = true;
			while (getline(s, token, ','))
			{
				if (first)
				{
					first = false;
					continue;
				}
				fd.SetCellWidth(stoi(token));
			}
		}
		else if (lineNumber >= 9u && lineNumber < 265u)
		{
			string token;
			istringstream s(line);

			bool first = true;
			while (getline(s, token, ','))
			{
				if (first)
				{
					first = false;
					continue;
				}
				data[0][lineNumber - 9u] = stoi(token);
			}
		}
		else if (lineNumber >= 265u && lineNumber < 521u)
		{
			string token;
			istringstream s(line);

			bool first = true;
			while (getline(s, token, ','))
			{
				if (first)
				{
					first = false;
					continue;
				}
				data[1][lineNumber - 265u] = stoi(token);
			}
		}

		++lineNumber;
	}

	fd.LoadBaseWidth(data[0]);
	fd.LoadWidthOffset(data[1]);

	fileStream.close();

	cout << "done." << endl;

	return fd;
}