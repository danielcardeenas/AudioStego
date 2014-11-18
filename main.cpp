#include <iostream>
#include "main.h"
#include "Algorithm.h"

using namespace std;

int main(int argc, char* argv[])
{
    string msg;
    string inputPath;
    string fileExt, inputExt;

    ifstream binStremFile;
    streampos binFileSize;
    vector<char> msgBuffer;

    int mode = 1; // hide

    if (argc < 2)
    {
        cout << "Where are my the parameters mate?" << endl;
        cout << "To hide a string:  ./HideMeIn [input_file] \"'string message'\" (Single quotation inside double quotation)" << endl;
        cout << "To hide a file:    ./HideMeIn [input_file] [file_to_hide]" << endl;
        cout << endl;
        cout << "To retrieve something already hidden:   ./HideMeIn [file_with_hidden_data] -f" << endl;
        return 0;
    }
    else if (argc == 2) {
        /**
        * When only the song path is passed.
        * Using the default message
        * @param: song
        * */

        cout << "No message to hide was specified. Using a default string message..." << endl;
        inputPath = argv[1];
        msg = "Default Message KEKLIFE";
        //return 0;
    }
    else if (argc == 3){
        /**
        * When only the song path and the (message or flag) are passed.
        * In case mode is passsed, find hidden message
        * @param: song, message/flag
        * */

        inputPath = argv[1];
        if(string(argv[2]) == "-f" || string(argv[2]) == "--find")
        {
            // Find the hidden message mode
            mode = 2;
        }
        else{
            // User wants to hide data then
            // Decide whether is a string or a file
            if (string(argv[2])[0] == 39 && string(argv[2]).back() == 39)
            {
                // Quotation marks detected. Hide string message
                mode = 1;
                msg = string(argv[2]);

                // Get input file extension
                inputExt = GetFileExtension(string(argv[1]));
            }
            else {
                // Take it as a file path
                mode = 3;

                // Get files extension
                fileExt = GetFileExtension(string(argv[2]));
                inputExt = GetFileExtension(string(argv[1]));

                binStremFile.open(string(argv[2]), std::ios::binary );
                binFileSize = binStremFile.tellg();
                msgBuffer.reserve(binFileSize); // Reserve the amount of the file size of memory to the vector
            }
        }
    }
    else{
        cout << "Too much argumments maybe? " << endl;

        cout << "To hide a string:  ./HideMeIn [input_file] \"'string message'\" (Single quotation inside double quotation)" << endl;
        cout << "To hide a file:    ./HideMeIn [input_file] [file_to_hide]" << endl;
        cout << endl;
        cout << "To retrieve something already hidden:   ./HideMeIn [file_with_hidden_data] -f" << endl;
        return 0;
    }

    cout << "Doing it boss! " << endl;

    // Get input file
    ifstream input(inputPath, std::ios::binary );
    if(!input.is_open())
    {
        cout << "Unable to open the file given" << endl;
        return 0;
    }

    // Copy the song into a buffer and closing it because we don't want to touch that
    vector<char> buffer(
                    (istreambuf_iterator<char>(input)),
                    (istreambuf_iterator<char>()));

    if (mode == 3)
    {
        /**
         * If we are going to hide a binary file:
         * Assign the binary file into a vector to play with him later and close the file.
         */

        msgBuffer.assign(
                (istreambuf_iterator<char>(binStremFile)),
                (istreambuf_iterator<char>()));

        binStremFile.close();
    }

    input.close();

    /**
    * mode:
    * 1 -> hide a string
    * 2 -> retrieve message (string or binary)
    * 3 -> hide a binary file
    **/

    if (mode == 1)
    {
        int status = PlayWithWaveBuffer(buffer, msg, inputExt);
        if (status == SUCCESS)
            cout << "Hiding process has finished successfully.\nCleaning memory..." << endl;
        else if (status == ERROR)
            cout << "Something failed.\nCleaning memory..." << endl;
    }
    else if (mode == 2){
        int status = FindHiddenMessage(buffer);
        if (status == SUCCESS)
            cout << "Recovering process has finished successfully.\nCleaning memory..." << endl;
        else if (status == ERROR)
            cout << "Something failed.\nCleaning memory..." << endl;
    }
    else if (mode == 3){
        int status = PlayWithWaveBuffer(buffer, msgBuffer, fileExt, inputExt);
        if (status == SUCCESS)
            cout << "Hiding process has finished successfully.\nCleaning memory..." << endl;
        else if (status == ERROR)
            cout << "Something failed.\nCleaning memory..." << endl;
    }

    // Force remove the buffer from memory
    vector<char>().swap(buffer);
    vector<char>().swap(msgBuffer);
    return 0;
}

/**
* This will return the extension even on strings like "a.b.c.d.e.s.mp3"
* If it cannot find the extension it will return "".
*
* @param fileName: The entire file name
* */
string GetFileExtension(const string& fileName)
{
    if(fileName.find_last_of(".") != std::string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}