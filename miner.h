#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "lib/utils.h"
#include "lib/entities.h"


void updateMinerBlock(BLOCK_T *minerBlock);
void generateMinerBlock(BLOCK_T *minerBlock, BLOCK_T *blockChain, int relayed_by);
//void *miner(int *thread_id);
//void *badMiner(int *thread_id);

#endif //BITCOIN_MINER_H
