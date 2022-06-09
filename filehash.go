package main

import (
	"bufio"
	"crypto/md5"
	"fmt"
	"os"
)

type File struct {
	name     string
	fileobj  os.File
	open     bool
	blocknum int
}

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

func openFile(fileName string) *File {
	var file File
	file.name = fileName
	file.blocknum = 0
	fileobj, err := os.Open(file.name)
	check(err)
	file.fileobj = *fileobj
	return &file
}

func closeFile(file File) {
	err := file.fileobj.Close()
	check(err)
}

func readnextChunk(file *File, chunkSize int) ([]byte, bool) {
	end := false
	_, err := file.fileobj.Seek(int64(chunkSize*file.blocknum), 0)
	check(err)
	chunk := make([]byte, chunkSize)
	br := bufio.NewReader(&file.fileobj)
	for i := 0; i < chunkSize; i++ {
		b, err := br.ReadByte()
		if b == 0 {
			end = true
		}
		check(err)
		chunk[i] = b
	}
	file.blocknum += 1
	return chunk, end
}

func hashChunk(block []byte) []byte {
	hash := md5.Sum(block)
	return hash[:8]
}

func readallChunks(file *File, chunkSize int) [][]byte {
	var file_end = false
	allchunks := [][]byte{}
	for !file_end {
		chunk, end := readnextChunk(file, chunkSize)
		allchunks = append(allchunks, chunk)
		file_end = end
	}
	return allchunks
}

func hashallChunks(allchunks [][]byte) [][]byte {
	allchunkHash := [][]byte{}
	for i := 0; i < len(allchunks); i++ {
		allchunkHash = append(allchunkHash, hashChunk(allchunks[i]))
	}
	return allchunkHash
}
