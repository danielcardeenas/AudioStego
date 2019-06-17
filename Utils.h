#pragma once

enum
{
	SUCCESS = 1,
	ERROR = 2,
};

string	GetFileExtension(const string& _fileName);

int		OutputBinFile(vector<char>&	buffer,
					  const string&	fileExtension);
