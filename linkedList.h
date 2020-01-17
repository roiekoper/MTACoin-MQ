#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_
#define _CRT_SECURE_NO_WARNINGS

#include "entities.h"

void push(node_t **head, BLOCK_T **new_block);
BLOCK_T *get_last_block(node_t *head);

#endif