#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "md5.h"
using namespace std;
//file class with file name, file dir, file size, file hash
class FileObj {
public:
	string name;
	int size;
	string hash;
	FileObj(string name)
	{
		this->name = name;
		this->updateHash();
		this->size = computeSize();
	}
	//~FileObj();
	void setHash(string hash) { this->hash = hash; }
	string getName() { return this->name; }
	int getSize() { return size; }
	string getHash() { return hash; }
	void updateHash() { this->hash = computeHash(); }
	int computeSize();
	bool hashChanged();

private:
	string computeHash();
	char* readFile();
};