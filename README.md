# MTACoin-MQ

## Applied By:
1. Roie Koper
2. Oz Hogeg

## Description:
Practice synchronization between consumer and producer threads in the same process.
Execute multithreads application that illustrates a cryptocurrency blockchain network in which there are 4 mining threads that mine blocks (calculate hashes) and send these blocks to a server that verifies and adds these blocks to its blockchain.


### Execution commands:
1. > `make`

2. > `sudo ./main.o`

### Known Issues:
- How to fix fatal error: `zlib.h: no such file or directory`?
    - `sudo apt-get install libz-dev`
- We chose not to separate server, miners & common functions to files.
- `Bad Miner` send an invalid block to the server every 1 second(sleeps rest of the time), the purpose of it is to test the server drops invalid blocks.
- We used `crc32` hash function to generate hash from block data.
