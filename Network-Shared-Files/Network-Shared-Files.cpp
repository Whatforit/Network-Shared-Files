#include <fstream>
#include <vector>
#include <filesystem>
#include "FileObj.h"
#include "md5.h"
#include <iostream>
#include <fstream>
using namespace std;
//list all files in a directory, return a vector of file names: mode = 0 for single, mode = 1 for recursive
vector<string> listFiles(string dir, int mode)
{
	vector<string> fileList;
	if (mode == 0) {
		for (auto& p : filesystem::directory_iterator(dir))
		{
			//cout << p.path().string();
			if (!p.is_directory() || p.path().extension() == ".hash" || p.path().extension() == ".back") {
				fileList.push_back(p.path().string());
			}
		}
	}
	else {
		for (auto& p : filesystem::recursive_directory_iterator(dir))
		{
			//cout << p.path().string();
			if (!p.is_directory()) {
				fileList.push_back(p.path().string());
			}
		}
	}
	return fileList;
}

//accept list of filenames, return a vector of FileObj
vector<FileObj> createFileObj(vector<string> fileList)
{
	vector<FileObj> fileObjList;
	for (int i = 0; i < fileList.size(); i++)
	{
		//fstream file;
		//file.open(fileList[i], ios::binary | ios::ate);
		FileObj fileObj(fileList[i]);
		//file.close();
		fileObjList.push_back(fileObj);
	}
	return fileObjList;
}

//create vector of file hashes
vector<string> createHashVector(vector<FileObj> fileObjList)
{
	vector<string> hashVector;
	for (int i = 0; i < fileObjList.size(); i++)
	{
		hashVector.push_back(fileObjList[i].getHash());
	}
	return hashVector;
}


//create vector of tuples of file names and hashes
vector<tuple<string, string>> createTupleVector(vector<FileObj> fileObjList)
{
	vector<tuple<string, string>> tupleVector;
	for (int i = 0; i < fileObjList.size(); i++)
	{
		tupleVector.push_back(make_tuple(fileObjList[i].getName(), fileObjList[i].getHash()));
	}
	return tupleVector;
}


//write vector of tuples to file
void writeHashFile(vector<tuple<string, string>> tupleVector, string fileName)
{
	ofstream file;
	file.open(fileName);
	for (int i = 0; i < tupleVector.size(); i++)
	{
		file << get<0>(tupleVector[i]) << "|" << get<1>(tupleVector[i]) << endl;
	}
	file.close();
}

//read vector of tuples from file
vector<tuple<string, string>> readHashFile(string fileName)
{
	vector<tuple<string, string>> tupleVector;
	ifstream file;
	file.open(fileName);
	string line;
	while (getline(file, line))
	{
		string name = line.substr(0, line.find("|"));
		string hash = line.substr(line.find("|") + 1);
		tupleVector.push_back(make_tuple(name, hash));
	}
	file.close();
	return tupleVector;
}

//return vector of filenames not in  vector

vector<string> findMissingFiles(vector<tuple<string, string>> tupleVector, vector<string> fileList)
{
	vector<string> missingFiles;
	for (int i = 0; i < fileList.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < tupleVector.size(); j++)
		{
			if (fileList[i] == get<0>(tupleVector[j]))
			{
				found = true;
			}
		}
		if (!found)
		{
			missingFiles.push_back(fileList[i]);
		}
	}
	return missingFiles;
}


//create directory
void createDir(string fileName)
{
	if (!filesystem::exists(fileName))
	{
		filesystem::create_directory(fileName);
	}
}
//check if file/directory exists
bool fileExists(string fileName)
{
	return filesystem::exists(fileName);
}
//create directories file
void createFile(string fileName)
{
	ofstream file;
	file.open(fileName);
	file.close();
}

//write vector of directories to file
void writeDirVector(vector<string> dirVector, string fileName)
{
	ofstream file;
	file.open(fileName);
	for (int i = 0; i < dirVector.size(); i++)
	{
		file << dirVector[i] << endl;
	}
	file.close();
}


//read list of directories from file
vector<string> readDirVector(string fileName)
{
	vector<string> dirVector;
	ifstream file;
	file.open(fileName);
	string line;
	while (getline(file, line))
	{
		dirVector.push_back(line);
	}
	file.close();
	return dirVector;
}

//sanitize user input, replace single backslash with double
string sanitize(string input)
{
	string output = "";
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == '\\')
		{
			output += "\\\\";
		}
		else
		{
			output += input[i];
		}
	}
	return output;
}

//get current working directory
string getCwd()
{
	return filesystem::current_path().string();
}
int main() {
	string cwd = getCwd();
	string dirDelim = "";
	if (cwd.find("\\") != string::npos)
	{
		dirDelim = "\\";
	}
	else
	{
		dirDelim = "/";
	}
	string nsfDir = cwd + dirDelim + "NSF";
	string nsfHashFile = nsfDir + dirDelim + "nsfHash.hash";
	string nsfBackFile = nsfDir + dirDelim + "nsfDir.back";
	cout << "NSF directory: " << nsfDir << endl;
	cout << "NSF hash file: " << nsfHashFile << endl;
	cout << "NSF directory file: " << nsfBackFile << endl;
	if (!fileExists(nsfDir)) {
		createDir(nsfDir);
		createFile(nsfHashFile);
		createFile(nsfBackFile);


	}

	cout << "Here are the files currently being Shared" << endl;
	cout << "<----------------------------------------------------->" << endl;
	vector<string> dirVector = readDirVector(nsfBackFile);
	for (int i = 0; i < dirVector.size(); i++)
	{
		cout << to_string(i + 1) + ": " << dirVector[i] << endl;
	}
	cout << "<----------------------------------------------------->" << endl;
	cout << "" << endl;
	cout << "Would you like to add a directory? (y/N): ";
	string input;
	cin >> input;
	vector<string> recursiveVec;
	if (input == "y") {
		cout << "Enter directories you'd like to add, then mode 0|1 (single|recursive) enter 'q' when done" << endl;
		string dir;
		int mode;
		while (1)
		{
			cin >> dir;
			if (dir == "q" || dir == "Q" || dir == "") {
				break;
			}
			cin >> mode;
			recursiveVec = listFiles(dir, mode);
			dirVector.insert(dirVector.begin(), recursiveVec.begin(), recursiveVec.end());

		}


		cout << "Here are the files currently being Shared" << endl;
		cout << "<----------------------------------------------------->" << endl;
		for (int i = 0; i < dirVector.size(); i++)
		{
			cout << to_string(i + 1) + ": " << dirVector[i] << endl;
		}
		cout << "<----------------------------------------------------->" << endl;
	}
	cout << "" << endl;
	cout << "Would you like to remove a file? (y/N): ";
	cin >> input;
	if (input == "y" || input == "Y") {
		cout << "Enter directory ids you'd like to remove, enter 'q' when done" << endl;
		string dir;
		while (1)
		{
			cin >> dir;
			if (dir == "q" || dir == "Q" || dir == "") {
				break;
			}
			dirVector.erase(dirVector.begin() + (stoi(dir) - 1));
		}
		cout << "Here are the files currently being Shared" << endl;
		cout << "<----------------------------------------------------->" << endl;
		for (int i = 0; i < dirVector.size(); i++)
		{
			cout << to_string(i + 1) + ": " << dirVector[i] << endl;
		}
		cout << "<----------------------------------------------------->" << endl;
		cout << "" << endl;
	}
	cout << "Writing changes to file..." << endl;
	writeDirVector(dirVector, nsfBackFile);
	cout << "Writing changes to file...done" << endl;

	vector<tuple<string, string>> nameHashTupVec = readHashFile(nsfHashFile);
	vector<string> nameHashVec;
	//create vector of filenames from tuplevector
	for (int i = 0; i < nameHashTupVec.size(); i++) {
		nameHashVec.push_back(get<0>(nameHashTupVec[i]));
	}

	//create file object vector from tupleVector
	vector<FileObj> fileObjVec = createFileObj(nameHashVec);


	vector<string> missingFiles = findMissingFiles(nameHashTupVec, dirVector);
	vector<FileObj> newFileObjVec = createFileObj(missingFiles);
	for (int i = 0; i < newFileObjVec.size(); i++) {
		newFileObjVec[i].updateHash();
	}

	fileObjVec.insert(fileObjVec.end(), newFileObjVec.begin(), newFileObjVec.end());

	//print fileObjVec
	for (int i = 0; i < fileObjVec.size(); i++)
	{
		cout << fileObjVec[i].getName() << endl;
		cout << fileObjVec[i].getHash() << endl;
	}

}

