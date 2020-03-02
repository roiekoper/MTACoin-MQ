# MTACoin-MQ

## Applied By:
1. Roie Koper
2. Oz Hogeg

## Description:
Practice multi process communication using POSIX message queues. <br>
Execute multithreads application that illustrates a cryptocurrency blockchain network in which there are 4 mining threads that mine blocks (calculate hashes) and send these blocks to a server that verifies and adds these blocks to its blockchain.


### Execution commands:
1. > `make`

####Launcher:
2. > `sudo ./build/launcher.out`
####Server:
2. > `sudo ./build/server.out`
####Miner:
2. > `sudo ./build/miner.out`

### Known Issues:
- How to fix fatal error: `zlib.h: no such file or directory`?
    - `sudo apt-get install libz-dev`
- We used `crc32` hash function to generate hash from block data.
