#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "entities.h"

void push(NODE_T **head, BLOCK_T **new_block);
BLOCK_T *get_last_block(NODE_T *head);

#endif