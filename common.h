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

#define assert_if(errnum)      \
    if (errnum != 0)           \
    {                          \
        printf("Error: %m\n"); \
        exit(EXIT_FAILURE);    \
    }

#define POLICY_STR(policy) (policy == SCHED_FIFO) ? "SCHED_FIFO" : (policy == SCHED_RR) ? "SCHED_RR" : (policy == SCHED_OTHER) ? "SCHED_OTHER" : "???"


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
