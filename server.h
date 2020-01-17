#ifndef BITCOIN_SERVER_H
#define BITCOIN_SERVER_H

#include "entities.h"
#include "common.h"
#include <pthread.h>

void *server(void *pVoid);
void generateInitBlock();
int isLegalBlock(BLOCK_T *serverBlock);

#endif //BITCOIN_SERVER_H
