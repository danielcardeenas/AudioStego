// guard header
#ifndef __ALGORITHM_H_INCLUDED__
#define __ALGORITHM_H_INCLUDED__

#define SUCCESS 1
#define ERROR 2
#define DEF_MODULE 64 // The bigger the module, the more spread(?) the message
#define WAV_HEADER 44
#define MY_HEADER_MODULE 64
#define START_SPACE 0 // Space skipped in order to dont put a message at the starts of the songs
/**
* My custom header with the attributes of the file to retireve:
* 4 bytes for modulus value
* 4 for file extension
* 1 byte for the type of message to find (text or binary)
* */
#define MY_HEADER 9

#include <vector>
#include <string>
#include <fstream>
#include "CustomHeader.h"

void CreateHeader(long& modulus, char* modulusBytes, char* customHeader, std::string& fileExtension, bool isBinaryType);
int OutputBinFile(std::vector<char>& buffer, CustomHeader& cHeader);
int OutputBindedData(std::vector<char>& buffer, std::string& extension);
int WriteMessageFromEnd(std::vector<char>& buffer, std::string msg);
int PlayWithWaveBuffer(std::vector<char>& buffer, std::string& msg, std::string& inputExt);             // Hide a string
int PlayWithWaveBuffer(std::vector<char>& buffer, std::vector<char>& fileMsg, std::string& fileExtension, std::string& inputExt);  // Hide a binary file
int FindHiddenTextInWave(std::vector<char>& buffer, CustomHeader& customHeader);
int FindHiddenBinaryInWave(std::vector<char>& buffer, CustomHeader& customHeader);
int FindHiddenMessage(std::vector<char>& buffer);

#endif // __ALGORITHM_H_INCLUDED__
