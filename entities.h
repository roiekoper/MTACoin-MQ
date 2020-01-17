#ifndef ENTITIES_H
#define ENTITIES_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>

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

BLOCK_T *newBlock = NULL;

#endif
