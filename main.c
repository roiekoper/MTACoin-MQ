#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include "lib/entities.h"
//#include <sys/fcntl.h>

int main()
{
    mqd_t mq;
    pthread_t wpid, rpid[NUM_OF_MINER];
    struct mq_attr attr = {0};

    /* initialize the queue attributes */
    attr.mq_maxmsg = MQ_MAX_SIZE;
    attr.mq_msgsize = MQ_MAX_MSG_SIZE;

    /* create the message queue and close(not delete) it immidiatly as it will be used only by children */
    mq_unlink(MQ_NAME); // delete first if already exists, this requires sudo privilege
    mq = mq_open(MQ_NAME, O_CREAT, S_IRWXU | S_IRWXG, &attr);

    /* create writer process */
    wpid = vfork();
    if (wpid == 0) //Writer
    {
        char *argv[] = {"./build/server.out", 0};
        execv("./build/server.out", argv);
    }

    /* Create reader process */
    for(int i = 0; i < NUM_OF_MINER; i++)
    {
        rpid[i] = vfork();
        if(rpid[i] == 0) //Reader
        {
            char *argv[] = {"./build/miner.out", 0};
            execv("./build/miner.out", argv);
        }
    }

    // Do nothing, only parent process should get here
    pause();

    return 0;
}
