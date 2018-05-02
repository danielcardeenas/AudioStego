#pragma once
#include "CustomHeader.h"
/**
* My custom header with the attributes of the file to retrieve:
* 4 bytes for step value
* 4 for file extension
* 1 byte for the type of message to find (text or binary)
* */
const int HEADER_SIZE	= 9;
const int HEADER_STEP	= 64;
const int HEADER_SIZEOF	= (HEADER_SIZE - 1) * HEADER_STEP;
const int DEFAULT_STEP	= 64;

const int WAV_HEADER	= 44;
const int START_SPACE	= 0;

int EncodeDataInBuffer	(vector<char>&	buffer,
						 vector<char>&	fileMsg,
						 const string&	fileExtension,
						 const string&	inputExt,
						 bool			isBinaryType);

int RetrieveData		(vector<char>& buffer);



