#include "stdafx.h"
#include "Algorithm.h"
#include "Utils.h"

void EncodeMessage(string inputPath, string msg)
{
	string	inputExt		= GetFileExtension(string(inputPath));
	bool	isBinaryFile	= false;
	vector<char> msgBuffer(msg.begin(), msg.end());

	cout << "Encoder(message) starting..." << endl;
	ifstream input(inputPath, ios::binary);
	if (!input.is_open())
	{
		cout << "Unable to open the file given" << endl;
		return;
	}

	vector<char> buffer((istreambuf_iterator<char>(input)),(istreambuf_iterator<char>()));
	input.close();
	cout << "File is buffered" << endl;

	int status = EncodeDataInBuffer(buffer, msgBuffer, string(), inputExt, isBinaryFile);
	if (status == SUCCESS)
		cout << "Hiding process has finished successfully.\nCleaning memory..." << endl;
	else if (status == ERROR)
			cout << "Something failed.\nCleaning memory..." << endl;

	vector<char>().swap(buffer);
	vector<char>().swap(msgBuffer);
	return ;
}

void EncodeFile(string inputPath, string fileToEncodePath)
{
	string	inputExt			= GetFileExtension(string(inputPath));
	string	fileExt				= GetFileExtension(string(fileToEncodePath));
	bool	isBinaryType		= true;	

	cout << "Encoder(file) starting..." << endl;

	ifstream input(inputPath, ios::binary);
	if (!input.is_open())
	{
		cout << "Unable to open container file" << endl;
		return;
	}
	vector<char> buffer((istreambuf_iterator<char>(input)),(istreambuf_iterator<char>()));
	input.close();
	ifstream binStreamFile(fileToEncodePath, ios::binary);
	if (!binStreamFile.is_open())
	{
		cout << "Unable to open data file" << endl;
		return;
	}
	vector<char> msgBuffer((istreambuf_iterator<char>(binStreamFile)),(istreambuf_iterator<char>()));
	binStreamFile.close();

	cout << "Files are buffered" << endl;

	int status = EncodeDataInBuffer(buffer, msgBuffer, fileExt, inputExt, isBinaryType);
	if (status == SUCCESS)
		cout << "Hiding process has finished successfully.\nCleaning memory..." << endl;
	else if (status == ERROR)
		cout << "Something failed.\nCleaning memory..." << endl;

	vector<char>().swap(buffer);
	vector<char>().swap(msgBuffer);
	return;
}

void DecodeFile(string inputPath)
{
	string inputExt		= GetFileExtension(string(inputPath));

	cout << "Decoder starting..." << endl;
	ifstream input(inputPath, ios::binary);
	if (!input.is_open())
	{
		cout << "Unable to open the file given" << endl;
		return;
	}
	vector<char> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
	input.close();
	cout << "File is buffered" << endl;
	
	int status = RetrieveData(buffer);
	if (status == SUCCESS)
		cout << "Recovering process has finished successfully.\nCleaning memory..." << endl;
	else if (status == ERROR)
		cout << "Something failed.\nCleaning memory..." << endl;
	vector<char> msgBuffer;
	vector<char>().swap(buffer);
	vector<char>().swap(msgBuffer);
	return;
}

int main(int argc, char* argv[])
{
	string msg;
	string inputPath;
	string fileToEncodePath;

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
	        }
	        else {
	            // Take it as a file path
	            mode = 3;
				fileToEncodePath = string(argv[2]);
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

	/**
	* mode:
	* 1 -> hide a string
	* 2 -> retrieve message (string or binary)
	* 3 -> hide a binary file
	**/

	if (mode == 1)
	{
		EncodeMessage(inputPath, msg);
	}
	else if (mode == 2)
	{
		DecodeFile(inputPath);
	}
	else if (mode == 3)
	{
		EncodeFile(inputPath, fileToEncodePath);
	}

	return 0;
}