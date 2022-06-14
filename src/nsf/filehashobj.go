package main

import (
	"fmt"
	"os"
)

//Struct for containing file info
type File struct {
	name      string
	fileOBJ   os.File
	hashOBJ   *FileHash
	open      bool
	blockNum  int
	chunkSize int
}

type FileHash struct {
	name     string
	fileOBJ  *os.File
	fileHash [][]byte
}

//Checks for errors, ignoring EOF
func check(err error) {
	if err != nil && err.Error() != "EOF" {
		fmt.Println(err)
		panic(err)
	}
}

//Opens file and initializes file struct
func openFile(fileName string, chunkSize int) (*File, **FileHash) {
	var file File
	file.name = fileName
	file.blockNum = 0
	file.chunkSize = chunkSize
	fileOBJ, err := os.Open(file.name)
	check(err)
	file.fileOBJ = *fileOBJ
	file.open = true
	var fileHash = createHashOBJ(&file)
	file.hashOBJ = fileHash
	return &file, &fileHash
}

//Closes file and sets structs "open" flag to false
func (file File) closeFile() {
	err := file.fileOBJ.Close()
	check(err)
	err = file.hashOBJ.fileOBJ.Close()
	check(err)
	file.open = false
}

func createHashOBJ(file *File) *FileHash {
	var fileHash FileHash
	fileHash.name = "hash-" //+ file.name
	hashFile, err := os.Create(fileHash.name)
	check(err)
	fileHash.fileOBJ = hashFile
	fileHash.fileHash = file.getAllHash()
	writeHashOBJ(&fileHash)
	return &fileHash
}

func writeHashOBJ(fileHash *FileHash) {
	for i := 0; i < len(fileHash.fileHash); i++ {
		_, err := fileHash.fileOBJ.Write(fileHash.fileHash[i])
		check(err)
	}

}
