# Network-Shared-Files
Share directories and files over a network, synced in real time.

# How it works

The program runs on two endpoints, waiting for any file or directory it's pointing at to be updated. Upon finding a change to a file, the program will
crunch down until it finds the smallest changes, known as "chunks", and send only those chunks to the other endpoint, at which time the program on the other end will seamlessly replace those edited chunks.
