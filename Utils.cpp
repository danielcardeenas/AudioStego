#include "stdafx.h"
#include "Utils.h"

string GetFileExtension(const string& _fileName)
{
	if (_fileName.find_last_of(".") != string::npos)
		return _fileName.substr(_fileName.find_last_of(".") + 1);
	return "";
}

int OutputBinFile(vector<char>&	_buffer,
				  const string&	_fileExtension)
{
	string fileName;
	if (_fileExtension == "")
		fileName = "output";
	else
		fileName = "output." + _fileExtension;

	ofstream output(fileName, ios::binary);
	output.write((const char*)&_buffer[0], _buffer.size());
	output.close();
	cout << "File has been saved as: " << fileName << endl;

	return SUCCESS;
}