//
// Created by Roie Koper on 26/12/2019.
//

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>

void push(NODE_T **head, BLOCK_T *new_block) {
    NODE_T *prev_block = (*head);
    *head = malloc(sizeof(NODE_T));
    (*head)->block = new_block;
    (*head)->prev  = prev_block;
}

BLOCK_T *get_last_block(NODE_T *head) {
    BLOCK_T *head_block = head->block;

    BLOCK_T *head_block_copy = malloc(sizeof(BLOCK_T));
    head_block_copy->difficulty = head_block->difficulty;
    head_block_copy->hash = head_block->hash;
    head_block_copy->height = head_block->height;
    head_block_copy->nonce = head_block->nonce;
    head_block_copy->prev_hash = head_block->prev_hash;
    head_block_copy->relayed_by = head_block->relayed_by;
    head_block_copy->timestamp = head_block->timestamp;

    return head_block_copy;
}