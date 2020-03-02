#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "lib/utils.h"
#include "lib/entities.h"


void updateMinerBlock(BLOCK_T *minerBlock);
void generateMinerBlock(BLOCK_T *minerBlock, BLOCK_T *blockChain, int relayed_by);
void minerBlockGenerated(struct mq_attr *mqNewBlocktAttr,
                         BLOCK_T *minerBlock,
                         int *isMinerBlockGenerate,
                         mqd_t *newBlock_mq);
#endif //BITCOIN_MINER_H
