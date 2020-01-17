#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h> // for crc32 method
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "entities.h"
#include "common.h"

#define NUM_OF_MINER 5
#define NUM_OF_GOOD_MINER 4

void *miner(int *thread_id);
void *badMiner(int *thread_id);
void updateMinerBlock(BLOCK_T *minerBlock);
BLOCK_T *generateMinerBlock(int relayed_by);

#endif //BITCOIN_MINER_H
