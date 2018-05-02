#include "stdafx.h"
#include "CustomHeader.h"

CustomHeader::CustomHeader(char* header)
{
	char* bytes = new char[4];
	for (int i = 0; i < 4; i++)
	{
		bytes[i] = header[i];
	}
	step = *reinterpret_cast<int*>(bytes);
	delete[] bytes;

	vector<char> ext;
	for (int i = 4; i < 8; i++)
	{
		if (header[i] == ' ')
		{
			break;
		}
		ext.push_back(header[i]);
	}
	extension = string(ext.begin(), ext.end());
	type = header[8];
}

int CustomHeader::Step()
{
	return step;
}

string CustomHeader::Extension()
{
	return extension;
}

char CustomHeader::Type()
{
	return type;
}