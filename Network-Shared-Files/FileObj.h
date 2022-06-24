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
	string md5;
	FileObj(string name) {
		this->name = name;
		this->updateHash();
		this->size = computeSize();
	}
	//~FileObj();
	void setHash(string hash) { this->md5 = md5; }
	string getName() { return this->name; }
	int getSize() { return size; }
	string getHash() { return md5; }
	void updateHash() { this->md5 = computeHash(); }
	int computeSize();

private:
	string computeHash();
	char* readFile();
};