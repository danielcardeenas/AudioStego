#include <iostream>
#include "Algorithm.h"

using namespace std;

int PlayWithWaveBuffer(vector<char>& buffer, string& msg, string& inputExt)
{
    char* modulusBytes = new char[4] {0}; // Max number of modulus in bytes
    char* customHeader = new char[MY_HEADER] {0}; // Custom header
    string fileExtension = "";

    string end = "@<;;";
    msg += end;

    // How many times the buffer is bigger than the message
    long modulus = ((buffer.size() - WAV_HEADER - START_SPACE) / (msg.size() + MY_HEADER));

    cout << "Spreading level: " << modulus << endl;

    // Verify if it is safe to hide the message. Must me at must half the size of the space avaible
    if(modulus <= 3)
    {
        cout << "The message might be to big for the audio file" << endl;
        return ERROR;
    }

    CreateHeader(modulus, modulusBytes, customHeader, fileExtension, false);

    int n = 0;
    int pos = 0;

    // Write my custom header first (Spread)
    for (vector<char>::iterator it = buffer.begin() + WAV_HEADER + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (n % MY_HEADER_MODULE == 0)
        {
            *it = customHeader[pos];
            pos++;
            //cout << "Header -> " << *it << endl; // Uncomment this to see the message being written

            if (pos == MY_HEADER)
            {
                cout << "Header wrote" << endl;
                break;
            }
        }
        n++;
    }

    // Delete arrays we are not using it anymore
    modulusBytes = new char[1]; // Relocate memory
    delete[] modulusBytes;
    delete[] customHeader;

    int j = 0;
    pos = 0;
    // Since the actual data of the wav starts at byte 44. Everything above is just header things that we don't care atm
    for (vector<char>::iterator it = buffer.begin() + WAV_HEADER + n + MY_HEADER_MODULE + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (j % modulus == 0)
        {
            *it = msg.at(pos);
            pos++;
            //cout << *it << endl; // Uncomment this to see the message being written

            if (pos >= msg.size())
                break;
        }
        j++;
    }

    if (pos < msg.size())
    {
        cout << "Maybe the whole file was not written in" << endl;
    }

    return OutputBindedData(buffer, inputExt);
}

int PlayWithWaveBuffer(vector<char>& buffer, vector<char>& msgBuffer, string& fileExtension, string& inputExt)
{
    char* modulusBytes = new char[4] {0}; // Max number of modulus in bytes
    char* customHeader = new char[MY_HEADER] {0}; // Custom header

    if (buffer.size()/4 <= msgBuffer.size())
    {
        cout << "The message might be to big for the audio file" << endl;
        return ERROR;
    }

    msgBuffer.push_back('@');
    msgBuffer.push_back('<');
    msgBuffer.push_back(';');
    msgBuffer.push_back(';');

    // How many times the buffer is bigger than the message
    // buffer.size() - (HEADER SIZE = 44 bytes) - (My own tags to de hidden file = 3 bytes)
    long modulus = ((buffer.size() - WAV_HEADER - START_SPACE) / (msgBuffer.size() + MY_HEADER));

    cout << "Spreading level: " << modulus << endl;

    // Verify if it is safe to hide the message. Must me at must half the size of the space avaible
    if(modulus <= 3)
    {
        cout << "The message might be to big for the audio file" << endl;
        return ERROR;
    }

    CreateHeader(modulus, modulusBytes, customHeader, fileExtension, true);

    int n = 0;
    int pos = 0;

    // Write my custom header first (Spread)
    for (vector<char>::iterator it = buffer.begin() + WAV_HEADER + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (n % MY_HEADER_MODULE == 0)
        {
            *it = customHeader[pos];
            pos++;
            //cout << "Header -> " << *it << endl; // Uncomment this to see the message being written

            if (pos == MY_HEADER)
            {
                cout << "Header wrote" << endl;
                break;
            }
        }
        n++;
    }

    // Delete arrays we are not using it anymore
    modulusBytes = new char[1]; // Relocate memory
    delete[] modulusBytes;
    delete[] customHeader;

    int j = 0;
    pos = 0;
    for (vector<char>::iterator it = buffer.begin() + WAV_HEADER + n + MY_HEADER_MODULE + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (j % modulus == 0)
        {
            *it = msgBuffer.at(pos);
            pos++;
            //cout << *it << endl; // Uncomment this to see the message being written

            if (pos >= msgBuffer.size())
                break;
        }
        j++;
    }

    if (pos < msgBuffer.size())
    {
        cout << "Maybe the whole file was not written in" << endl;
    }

    return OutputBindedData(buffer, inputExt);
}

/**
* Look for the custom header and define what type of message is in it
*
* @param buffer: File loaded into a buffer
* */
int FindHiddenMessage(vector<char>& buffer)
{
    char* customHeader = new char[MY_HEADER] {0}; // Custom header

    int n = 0;
    int pos = 0;
    cout << "Looking for the hidden message..." << endl;
    // Since the actual data of the wav starts at byte 44 we start from it. Everything above is just header things that we don't care atm
    for (vector<char>::iterator it = buffer.begin() + WAV_HEADER + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (n % MY_HEADER_MODULE == 0)
        {
            customHeader[pos] = *it;
            //cout << "Custom Header: " << customHeader[pos] << endl; // Uncomment this if you want to see the header being read
            pos++;
            if (pos == MY_HEADER)
            {
                //cout << "Header has been read " << endl;
                break;
            }
        }
        n++;
    }

    CustomHeader cHeader (customHeader);

    // Clean memory
    delete[] customHeader;

    if (cHeader.GetType() == 'b')
    {
        cout << "File detected. Retrieving it..." << endl;
        cHeader.SetLastPosition(n);
        return FindHiddenBinaryInWave(buffer, cHeader);
    }
    else if (cHeader.GetType() == 't'){
        cout << "String detected. Retrieving it..." << endl;
        cHeader.SetLastPosition(n);
        return FindHiddenTextInWave(buffer, cHeader);
    }
    else{
        // If it hits here it's because there was no message found in the file
        cout << "Failed to detect a hidden file." << endl;
        cout << "No custom header was found." << endl;
        return ERROR;
    }
}

int FindHiddenTextInWave(vector<char>& buffer, CustomHeader& customHeader)
{
    string msgText;

    int modulus = customHeader.GetModulus();
    int lastPos = customHeader.GetLastPosition();

    int n = 0;
    int pos = 0;
    vector<char>::iterator tempIterator;
    // Since the actual data of the wav starts at byte 44. Everything above is just header things that we don't care atm
    for (vector<char>::iterator it = buffer.begin() + WAV_HEADER + lastPos + MY_HEADER_MODULE + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (n % modulus == 0)
        {
            if (*it == 64)
            {
                // @
                //Setting the iterator to the next possible position
                tempIterator = buffer.begin() + n + 44 + MY_HEADER_MODULE + START_SPACE + lastPos + modulus;

                if (*tempIterator == 60) {
                    //<
                    //Setting the iterator to the next possible position
                    tempIterator = buffer.begin() + n + 44 + MY_HEADER_MODULE + START_SPACE + lastPos + (2* modulus);

                    if (*tempIterator == 59) {
                        // ;
                        // Setting the iterator to next possible flag
                        tempIterator = buffer.begin() + n + 44 + MY_HEADER_MODULE + START_SPACE + lastPos + (3 * modulus);

                        if (*tempIterator == 59){
                            // End of message reached
                            cout << "Message recovered size: " << pos << " bytes" << endl;

                            // Output text
                            cout << "Message: " << msgText.c_str() << endl;
                            return SUCCESS;
                        }
                    }
                }
            }

            msgText[pos] = *it;
            //cout << msgText[pos] << endl; // Uncomment this if yuo want to see the characters being read
            pos++;

        }
        n++;
    }

    // If it hits here it's because there was no message found in the file
    cout << "No message found :(" << endl;
    return ERROR;

}

int FindHiddenBinaryInWave(vector<char>& buffer, CustomHeader& customHeader)
{
    vector<char> msgBuffer;

    int modulus = customHeader.GetModulus();
    int lastPos = customHeader.GetLastPosition();

    int n = 0;
    int pos = 0;
    vector<char>::iterator tempIterator;
    // Since the actual data of the wav starts at byte 44. Everything above is just header things that we don't care atm
    for (vector<char>::iterator it = buffer.begin() + + WAV_HEADER + lastPos + MY_HEADER_MODULE + START_SPACE;
         it != buffer.end(); ++it)
    {
        if (n % modulus == 0)
        {
            if (*it == 64)
            {
                // @
                //Setting the iterator to the next possible position
                tempIterator = buffer.begin() + n + 44 + MY_HEADER_MODULE + START_SPACE + lastPos + modulus;

                if (*tempIterator == 60) {
                    //<
                    //Setting the iterator to the next possible position
                    tempIterator = buffer.begin() + n + 44 + MY_HEADER_MODULE + START_SPACE + lastPos + (2* modulus);

                    if (*tempIterator == 59) {
                        // ;
                        // Setting the iterator to next possible flag
                        tempIterator = buffer.begin() + n + 44 + MY_HEADER_MODULE + START_SPACE + lastPos + (3 * modulus);

                        if (*tempIterator == 59){
                            // End of message reached
                            cout << "Message recovered size: " << pos << " bytes" << endl;
                            return OutputBinFile(msgBuffer, customHeader);
                        }
                    }
                }
            }

            msgBuffer.push_back(*it);
            //cout << "Data recovered: " << msgBuffer[pos] << endl; // Uncomment this if you want to see the characters being read
            pos++;

        }
        n++;
    }

    // If it hits here it's because there was no message found in the file
    cout << "Could not find the end tags of the hidden file :(" << endl;
    return ERROR;
}

int WriteMessageFromEnd(vector<char>& buffer, string msg)
{
    // Verify if it is safe to hide the message in the buffer
    if ((buffer.size() / 4) < msg.size())
    {
        cout << "The message might be to big for the audio file" << endl;
        return ERROR;
    }

    // Inverse iterator runs backwards
    int n = 0;
    int pos = 0;
    for (vector<char>::reverse_iterator i = buffer.rbegin();
         i != buffer.rend(); ++i)
    {
        if (n%1000 == 0)
        {
            *i = msg.at(pos);
            pos++;
            cout << *i << endl;

            if (pos >= msg.size())
                break;
        }
        n++;
    }

    return SUCCESS;
}

int OutputBindedData(vector<char>& buffer, string& fileExtension)
{
    string fileName;
    if (fileExtension == "")
        fileName = "output";
    else
        fileName = "output." + fileExtension;

    ofstream output(fileName, std::ios::binary);
    output.write((const char*)&buffer[0], buffer.size());
    output.close();
    cout << "File has been saved as: " << fileName << endl;

    return SUCCESS;
}

int OutputBinFile(vector<char>& buffer, CustomHeader& cHeader)
{
    string fileName;
    if (cHeader.GetExtension() == "")
        fileName = "output";
    else
        fileName = "output." + cHeader.GetExtension();

    ofstream output(fileName, std::ios::binary);
    output.write((const char*)&buffer[0], buffer.size());
    output.close();
    cout << "File has been saved as: " << fileName << endl;

    return SUCCESS;
}

void CreateHeader(long& modulus, char* modulusBytes, char* customHeader, string& fileExtension, bool isBinaryType)
{
    /**
    * Create custom header (9 bytes):
    * Modulus value: 4 bytes
    * File extension: 4 bytes
    * Message type (text or file): 1 byte
    * */

    // If modulus is lesser than the max int value of 4 bytes assign it.
    // Otherwise, set a flag to use DEF_MODULE constant
    if (modulus <= 42946729)
        modulusBytes = reinterpret_cast<char*>(&modulus);
    else
        modulusBytes[0] = DEF_MODULE;

    int y, i;
    // Assign modulus to header
    for (i = 0; i < 4; i++)
        customHeader[i] = modulusBytes[i];

    // Assign the file extension to recover later
    for(y = 0, i = 4; i < 8; i++)
    {
        // Dinamically fill the 4 bytes of the file extension even if its less than 4 characters
        if(fileExtension.size() > y )
            customHeader[i] = fileExtension.at(y);
        else
            customHeader[i] = ' ';
        y++;
    }

    // Assign the type of the message
    if (isBinaryType)
        customHeader[8] = 'b';
    else
        customHeader[8] = 't';
}