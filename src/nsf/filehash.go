package main

import (
	"bufio"
	"crypto/md5"
)

//Reads next chunk bytes
func readNextChunk(file *File) ([]byte, bool) {
	end := false
	_, err := file.fileOBJ.Seek(int64(file.chunkSize*file.blockNum), 0) // seeks to next chunk not read
	check(err)
	chunk := make([]byte, file.chunkSize)
	br := bufio.NewReader(&file.fileOBJ)
	for i := 0; i < file.chunkSize; i++ {
		b, err := br.ReadByte()
		if b == 0 {
			end = true //sets end flag to determine the end of file
		}
		check(err)
		chunk[i] = b //sets the byte of the slice
	}
	file.blockNum += 1 //increments every chunk, determining which chunks have been read in
	return chunk, end
}

//Hashes a single chunk, rturning truncated md5 hash
func hashChunk(block []byte) []byte {
	hash := md5.Sum(block)
	return hash[:8]
}

//Reads all chunks in a file, returning array of []byte
func (file File) readAllChunks() [][]byte {
	var file_end = false
	allChunks := [][]byte{}
	for !file_end { //continues until getting eof signal from readNextChunk
		chunk, end := readNextChunk(&file)
		allChunks = append(allChunks, chunk)
		file_end = end
	}
	return allChunks
}

//Hashes all the chunks in a [][]byte array, returning [][]byte of hashes
func (file File) hashAllChunks(allchunks [][]byte) [][]byte {
	allChunkHash := [][]byte{}
	for i := 0; i < len(allchunks); i++ {
		allChunkHash = append(allChunkHash, hashChunk(allchunks[i]))
	}
	return allChunkHash
}

func (file File) getNextChunk() []byte {
	nextchunk, _ := readNextChunk(&file)
	return nextchunk
}

func (file File) getAllChunks() [][]byte {
	return file.readAllChunks()
}

func (file File) getAllHash() [][]byte {
	return file.hashAllChunks(file.readAllChunks())
}
