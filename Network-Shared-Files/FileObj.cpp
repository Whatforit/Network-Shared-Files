#include "FileObj.h"
#include <vector>
#include "md5.h"
#include <iostream>
//open file as bytes and return char[]
char* FileObj::readFile()
{
	ifstream file(this->name, ios::binary);
	if (!file)
	{
		std::cout << "File not found: " << this->name << std::endl;
		return NULL;
	}
	file.seekg(0, ios::end);
	int length = file.tellg();
	file.seekg(0, ios::beg);
	char* buffer = new char[length];
	file.read(buffer, length);
	file.close();
	return buffer;
}
string FileObj::computeHash()
{
	MD5 md5;
	char* fileContents = this->readFile();
	md5(fileContents, this->computeSize());
	return md5.getHash();
}

//computes file size
int FileObj::computeSize()
{
	ifstream file(this->name, ios::binary);
	if (!file)
	{
		std::cout << "File not found" << std::endl;
		return 0;
	}
	file.seekg(0, ios::end);
	int length = file.tellg();
	file.seekg(0, ios::beg);
	file.close();
	return length;
}

bool FileObj::hashChanged() {
	return (this->computeHash() == this->hash);
}


