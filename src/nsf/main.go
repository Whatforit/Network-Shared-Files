package main

//import "fmt"

func main() {
	file, _ := openFile("2022-05-16.txt", 16)
	/*allChunks := file.getAllChunks()
	fmt.Println(allChunks)
	allChunkHash := file.getAllHash()
	fmt.Println(allChunkHash)
	*/
	//fmt.Println(file.hashOBJ.fileHash)

	file.closeFile()
}
