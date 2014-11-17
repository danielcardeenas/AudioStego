#include <string>
#include <boost/algorithm/string.hpp>
#include "CustomHeader.h"

CustomHeader::CustomHeader(char* header) {
    // Set modulus first
    char* bytes = new char[4];
    for (int i = 0; i < 4; i++)
        bytes[i] = header[i];

    mModulus = *reinterpret_cast<int*>(bytes);

    // Set extension
    char* ext = new char[4];
    for (int y = 0, i = 4; i < 8; i++, y++)
        ext[y] = header[i];

    mExtension = std::string(ext);
    boost::algorithm::trim(mExtension);

    // Set type
    mType = header[8];

    delete[] bytes;
    delete[] ext;
}

CustomHeader::CustomHeader() {
    // Initialize the variables
    return;
}

int CustomHeader::GetModulus() {
    return this->mModulus;
}

std::string CustomHeader::GetExtension() {
    return this->mExtension;
}

char CustomHeader::GetType() {
    return this->mType;
}

int CustomHeader::GetLastPosition() {
    return this->mLastPos;
}

void CustomHeader::SetLastPosition(int n) {
    this->mLastPos = n;
}