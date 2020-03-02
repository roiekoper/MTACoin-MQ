#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "lib/utils.h"
#include "lib/entities.h"
#include <fcntl.h>

void generateMQ(const char *miner_que_name, struct mq_attr *mqInitAttr, CONNECTION_REQUEST_MESSAGE *connection_msg,
                int miner_id, mqd_t *newBlock_mq, mqd_t *miner_mq);
void updateMinerBlock(BLOCK_T *minerBlock);
void generateMinerBlock(BLOCK_T *minerBlock, BLOCK_T *blockChain, int relayed_by);

void minerBlockGenerated(struct mq_attr *mqNewBlocktAttr,
                         BLOCK_T *minerBlock,
                         int *isMinerBlockGenerate,
                         mqd_t *newBlock_mq);

void generateMQ(const char *miner_que_name,
                struct mq_attr *mqInitAttr,
                CONNECTION_REQUEST_MESSAGE *connection_msg,
                int miner_id,
                mqd_t *newBlock_mq,
                mqd_t *miner_mq);

void receivedBlockFromServer(int miner_id,
                             struct mq_attr *mqMinertAttr,
                             BLOCK_T *minerBlock,
                             int *isMinerBlockGenerate,
                             mqd_t *miner_mq);


#endif //BITCOIN_MINER_H
