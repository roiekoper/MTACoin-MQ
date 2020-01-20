#ifndef ENTITIES_H
#define ENTITIES_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "entities.h"
#include <string.h>
#include <zlib.h> // for crc32 method
#include <pthread.h>
#include <time.h>

#define NUM_OF_MINER 5
#define NUM_OF_GOOD_MINER 4

#define assert_if(errnum)      \
    if (errnum != 0)           \
    {                          \
        printf("Error: %m\n"); \
        exit(EXIT_FAILURE);    \
    }

#define POLICY_STR(policy) (policy == SCHED_FIFO) ? "SCHED_FIFO" : (policy == SCHED_RR) ? "SCHED_RR" : (policy == SCHED_OTHER) ? "SCHED_OTHER" : "???"

typedef struct {
    int height;        // Incremental ID of the block in the chain
    int timestamp;    // Time of the mine in seconds since epoch
    unsigned long hash;        // Current block hash value
    unsigned long prev_hash;    // Hash value of the previous block
    int difficulty;    // Amount of preceding zeros in the hash
    int nonce;        // Incremental integer to change the hash value
    int relayed_by;    // Miner ID
} BLOCK_T;

typedef struct node {
    BLOCK_T * block;
    struct node * prev;
} node_t;

node_t *block_chain_head = NULL;
int mask;
BLOCK_T *newBlock = NULL;
pthread_t miners[NUM_OF_MINER];
pthread_t server_thread;
int indices[NUM_OF_MINER];

pthread_mutex_t chain_lock;
pthread_cond_t new_block_cond;

#endif
