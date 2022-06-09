package main

import (
	"bufio"
	"crypto/md5"
	"fmt"
	"os"
)
//Struct for containing file info
type File struct {
	name     string
	fileobj  os.File
	open     bool
	blocknum int
}

//Checks for errors, ignoring EOF
func check(err error) {
	if err != nil && err.Error() != "EOF" {
		panic(err)
	}
}

func main() {
	file := openFile("2022-05-16.txt")
	allchunks := readallChunks(file, 16)
	fmt.Println(allchunks)
	allchunkhash := hashallChunks(allchunks)
	fmt.Println(allchunkhash)
	closeFile(*file)
}

//Opens file and initializes file struct
func openFile(fileName string) *File {
	var file File
	file.name = fileName
	file.blocknum = 0
	fileobj, err := os.Open(file.name)
	check(err)
	file.fileobj = *fileobj
	file.open = true
	return &file
}
//Closes file and sets structs "open" flag to false
func closeFile(file File) {
	err := file.fileobj.Close()
	check(err)
	file.open = false
}
//Reads next chunk bytes
func readnextChunk(file *File, chunkSize int) ([]byte, bool) {
	end := false
	_, err := file.fileobj.Seek(int64(chunkSize*file.blocknum), 0) // seeks to next chunk not read
	check(err)
	chunk := make([]byte, chunkSize)
	br := bufio.NewReader(&file.fileobj)
	for i := 0; i < chunkSize; i++ {
		b, err := br.ReadByte()
		if b == 0 {
			end = true //sets end flag to determine the end of file
		}
		check(err)
		chunk[i] = b //sets the byte of the slice
	}
	file.blocknum += 1 //increments every chunk, dtermining which chunks have been read in
	return chunk, end
}
//Hashes a single chunk, rturning truncated md5 hash
func hashChunk(block []byte) []byte {
	hash := md5.Sum(block)
	return hash[:8]
}

//Reads all chunks in a file, returning array of []byte
func readallChunks(file *File, chunkSize int) [][]byte {
	var file_end = false
	allchunks := [][]byte{}
	for !file_end {    //continues until getting eof signal from readnextChunk
		chunk, end := readnextChunk(file, chunkSize)
		allchunks = append(allchunks, chunk)
		file_end = end
	}
	return allchunks
}

//Hashes all the chunks in a [][]byte array, returning [][]byte of hashes
func hashallChunks(allchunks [][]byte) [][]byte {
	allchunkHash := [][]byte{}
	for i := 0; i < len(allchunks); i++ {
		allchunkHash = append(allchunkHash, hashChunk(allchunks[i]))
	}
	return allchunkHash
}
