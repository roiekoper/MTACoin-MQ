#ifndef BITCOIN_SERVER_H
#define BITCOIN_SERVER_H

#define _GNU_SOURCE
#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

void *server(void *pVoid);
void generateInitBlock();
int isLegalBlock(BLOCK_T *serverBlock);

#endif //BITCOIN_SERVER_H
