#ifndef BITCOIN_UTILS_H
#define BITCOIN_UTILS_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "linkedList.h"

#define NUM_OF_ZERO 16
#define MAX_STRING_SIZE 256

unsigned long generateHashFromBlock(BLOCK_T *block);
void print_block(BLOCK_T *block);
void delete_list();
void generateMask();

#endif //BITCOIN_UTILS_H
