#include "stdafx.h"
#include "Algorithm.h"
#include "Utils.h"

void CreateHeader(char*			_customHeader,
				  long			_step,
				  const string&	_fileExtension,
				  bool			_isBinaryType)
{
	const int STEP_SIZE = 4;
	union
	{
		char Bytes[STEP_SIZE];
		long Long;
	} step;

	if (_step <= 42946729)
	{
		step.Long = _step;
	}
	else
	{
		step.Bytes[0]	= DEFAULT_STEP;
		for (int i = 1; i < STEP_SIZE; i++)
			step.Bytes[1] = 0;
	}

	for (int i = 0; i < STEP_SIZE; i++)
		_customHeader[i] = step.Bytes[i];

	size_t counter = 0;
	for (int i = 4; i < 8; i++)
	{
		if (_fileExtension.size() > counter)
			_customHeader[i] = _fileExtension.at(counter);
		else
			_customHeader[i] = ' ';
		counter++;
	}

	if (_isBinaryType)
		_customHeader[8] = 'b';
	else
		_customHeader[8] = 't';
}

void WriteHeader(vector<char>&	_buffer,
				 char*			_customHeader)
{
	int shift	= 0;
	int pos		= 0;

	for (vector<char>::iterator it = _buffer.begin() + WAV_HEADER + START_SPACE;
		it != _buffer.end(); ++it)
	{
		if (shift % HEADER_STEP == 0)
		{
			*it = _customHeader[pos];
			pos++;

			if (pos == HEADER_SIZE)
			{
				cout << "Header wrote" << endl;
				break;
			}
		}
		shift++;
	}
}

void WriteBody(vector<char>&	_buffer,
			   long				_step,
			   vector<char>&	_msgBuffer)
{
	vector<char>::iterator it = _buffer.begin() + WAV_HEADER + HEADER_STEP + START_SPACE + HEADER_SIZEOF;

	int j = 0;
	size_t pos = 0;
	for (it; it != _buffer.end(); ++it)
	{
		if (j % _step == 0)
		{
			*it = _msgBuffer.at(pos);
			pos++;
			if (pos >= _msgBuffer.size())
				break;
		}
		j++;
	}

	if (pos < _msgBuffer.size())
	{
		cout << "Maybe the whole file was not written in" << endl;
	}
}

int EncodeDataInBuffer(vector<char>&	_buffer,
					   vector<char>&	_data,
					   const string&	_fileExtension,
					   const string&	_inputExt,
					   bool				_isBinaryType)
{
	char* customHeader = new char[HEADER_SIZE] {0};

	_data.push_back('@');
	_data.push_back('<');
	_data.push_back(';');
	_data.push_back(';');

	long step = ((_buffer.size() - WAV_HEADER - START_SPACE) / (_data.size() + HEADER_SIZE));
	cout << "Step: " << step << endl;
	if (step <= 3)
	{
		cout << "The message might be to big for the audio file" << endl;
		return ERROR;
	}

	CreateHeader(customHeader, step, _fileExtension, _isBinaryType);
	WriteHeader(_buffer, customHeader);
	WriteBody(_buffer, step, _data);

	delete[] customHeader;
	return OutputBinFile(_buffer, _inputExt);
}

/////////////////////////////////////////////////////////////////////////

void ReadHeader(vector<char>& _buffer, char* _customHeader)
{
	int shift = 0;
	int pos = 0;
	vector<char>::iterator it = _buffer.begin() + WAV_HEADER + START_SPACE;
	for (it; it != _buffer.end(); ++it)
	{
		if (shift % HEADER_STEP == 0)
		{
			_customHeader[pos] = *it;
			pos++;
			if (pos == HEADER_SIZE)
			{
				break;
			}
		}
		shift++;
	}
}

int ReadData(vector<char>&	_buffer,
			 CustomHeader&	_customHeader,
			 vector<char>&	_msgBuffer)
{
	int step = _customHeader.Step();
	int shift = 0;
	int pos = 0;
	vector<char>::iterator tempIterator;
	for (vector<char>::iterator it = _buffer.begin() + WAV_HEADER + HEADER_SIZEOF + HEADER_STEP + START_SPACE;
		it != _buffer.end(); ++it)
	{
		if (shift % step == 0)
		{
			if (*it == 64)
			{
				// @
				//Setting the iterator to the next possible position
				tempIterator = _buffer.begin() + shift + 44 + HEADER_STEP + START_SPACE + HEADER_SIZEOF + step;

				if (*tempIterator == 60)
				{
					//<
					//Setting the iterator to the next possible position
					tempIterator = _buffer.begin() + shift + 44 + HEADER_STEP + START_SPACE + HEADER_SIZEOF + (2 * step);

					if (*tempIterator == 59)
					{
						// ;
						// Setting the iterator to next possible flag
						tempIterator = _buffer.begin() + shift + 44 + HEADER_STEP + START_SPACE + HEADER_SIZEOF + (3 * step);

						if (*tempIterator == 59)
						{
							// End of message reached
							cout << "Message recovered size: " << pos << " bytes" << endl;
							return SUCCESS;
						}
					}
				}
			}

			_msgBuffer.push_back(*it);
			pos++;

		}
		shift++;
	}

	cout << "Could not find the end tags of the hidden file :(" << endl;
	return ERROR;
}

int RetrieveData(vector<char>& _buffer)
{
	char* customHeader	= new char[HEADER_SIZE] {0};
	vector<char> data;
	cout << "Looking for the hidden message..." << endl;
	ReadHeader(_buffer, customHeader);
	CustomHeader cHeader(customHeader);
	delete[] customHeader;
	int result = ReadData(_buffer, cHeader, data);
	

	if (cHeader.Type() == 'b')
	{
		cout << "File detected. Retrieving it..." << endl;
		return OutputBinFile(data, cHeader.Extension());
	}
	else if (cHeader.Type() == 't')
	{
		cout << "String detected. Retrieving it..." << endl;
		cout << "Message:" << endl;
		for (size_t i = 0; i < data.size(); i++)
		{
			cout << data[i];
		}
		cout << endl;
		return SUCCESS;
	}
	else
	{
		cout << "Failed to detect a hidden file." << endl;
		cout << "No custom header was found." << endl;
		return ERROR;
	}
}