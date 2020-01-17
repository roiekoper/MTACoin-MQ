#ifndef BITCOIN_COMMON_H
#define BITCOIN_COMMON_H

#define _GNU_SOURCE

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include <string.h>
#include <zlib.h> // for crc32 method

#define NUM_OF_MINER 5
#define NUM_OF_GOOD_MINER 4
#define NUM_OF_ZERO 16
#define MAX_STRING_SIZE 256

unsigned long generateHashFromBlock(BLOCK_T *block);
node_t *block_chain_head = NULL;
int mask;
void generateMask();
void print_block(BLOCK_T *block);
void delete_list();

pthread_t miners[NUM_OF_MINER];
pthread_t server_thread;
int indices[NUM_OF_MINER];

pthread_mutex_t chain_lock;
pthread_cond_t new_block_cond;

#endif //BITCOIN_COMMON_H
