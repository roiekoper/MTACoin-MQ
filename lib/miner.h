#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

void *miner(int *thread_id);
void *badMiner(int *thread_id);
void updateMinerBlock(BLOCK_T *minerBlock);
BLOCK_T *generateMinerBlock(int relayed_by);

#endif //BITCOIN_MINER_H
