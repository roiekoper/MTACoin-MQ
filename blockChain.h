#ifndef BITCOIN_BLOCK_CHAIN_H
#define BITCOIN_BLOCK_CHAIN_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "entities.h"
#include "server.h"
#include "miner.h"

void startBlockChainMQ();
void generateMask();

#endif //BITCOIN_BLOCK_CHAIN_H