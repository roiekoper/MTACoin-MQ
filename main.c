//#include "lib/blockChain.h"

#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
//#include <sys/fcntl.h>

// ------- mq_ipc_vfork_include

#define MQ_MAX_SIZE         10
#define MQ_MAX_MSG_SIZE     100 		//Some big value(in bytes)
#define MQ_NAME             "/my_mq"

typedef enum{
    FLOAT,
    UINT
} NUMBER_TYPE_E;

/* Data that will be passed from the Writer to the reader
should hold the actual application data */
typedef struct msg{
    NUMBER_TYPE_E type;
    char data[]; // Dynamic/flexible array - place holder for unknown size data
}MSG_T;

typedef struct float_msg_data{
    float fvalue;
} FLOAT_MSG_DATA_T;

typedef struct uint_msg_data{
    unsigned int uvalue;
} UINT_MSG_DATA_T;

// ------- mq_ipc_vfork_include

#define NUM_OF_READERS    2

int main()
{
    mqd_t mq;
    pthread_t wpid, rpid[NUM_OF_READERS];
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
        char *argv[] = {"./mq_ipc_vfork_writer.out", 0};
        execv("./mq_ipc_vfork_writer.out", argv);
    }

    /* Create reader process */
    for(int i = 0; i < NUM_OF_READERS; i++)
    {
        rpid[i] = vfork();
        if(rpid[i] == 0) //Reader
        {
            char *argv[] = {"./mq_ipc_vfork_reader.out", 0};
            execv("./mq_ipc_vfork_reader.out", argv);
        }
    }

    // Do nothing, only parent process should get here
    pause();

    return 0;
}