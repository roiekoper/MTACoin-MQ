#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "common.h"
#include "miner.h"
#include "server.h"

// Functions Declaration
void createThreads();

int main()
{
    pthread_mutex_init(&chain_lock, NULL);
    pthread_cond_init(&new_block_cond, NULL);
    generateMask();
    createThreads();
    return 0;
}

void createThreads()
{
    int res = -1, i = 0;
    pthread_attr_t attr;
    pthread_attr_t attr2;
    struct sched_param miners_priority = {60};
    struct sched_param server_priority = {90};

    res = pthread_attr_init(&attr);
    assert_if(res);

    res = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    assert_if(res);

    res = pthread_attr_setschedparam(&attr, &miners_priority);
    assert_if(res);

    res = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    assert_if(res);

    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    pthread_attr_setschedparam(&attr, &miners_priority);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    for (i = 0; i < NUM_OF_MINER; i++)
    {
        indices[i] = i + 1;
        pthread_create(&miners[i], &attr, (void *(*)(void *))(i < NUM_OF_GOOD_MINER ? miner : badMiner), &indices[i]);
    }

    pthread_attr_init(&attr2);

    res = pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
    assert_if(res);

    res = pthread_attr_setschedparam(&attr2, &server_priority);
    assert_if(res);

    res = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    assert_if(res);

    pthread_create(&server_thread, &attr2, server, NULL);
    pthread_join(server_thread, NULL);

    for (i = 0; i < NUM_OF_MINER; i++)
        pthread_join(miners[i], NULL);
}