#include "server.h"

void main() {
    mqd_t miners_mq[NUM_OF_MINER];
    struct mq_attr mq_connection_request_attr = {0};
    struct mq_attr mq_new_block_attr = {0};

    /* initialize the queue attributes */
    mq_connection_request_attr.mq_maxmsg = MQ_MAX_SIZE;
    mq_connection_request_attr.mq_msgsize = MQ_MAX_MSG_SIZE;

    /* initialize the queue attributes */
    mq_new_block_attr.mq_maxmsg = MQ_MAX_SIZE;
    mq_new_block_attr.mq_msgsize = MQ_MAX_MSG_SIZE;

    //printf("Server generate new block to chain\n");
    generateInitBlock();

    mq_unlink(MQ_CONNECTION_REQUEST_NAME); // delete first if already exists, this requires sudo privilege
    mqd_t connection_mq = mq_open(MQ_CONNECTION_REQUEST_NAME, O_CREAT, S_IRWXU | S_IRWXG, &mq_connection_request_attr);

    mq_unlink(MQ_NEW_BLOCK_NAME);
    mqd_t newBlock_mq = mq_open(MQ_NEW_BLOCK_NAME, O_CREAT, S_IRWXU | S_IRWXG, &mq_new_block_attr);

    //printf("Server generate all ques\n");

    struct mq_attr mqAttr = {0};

    BLOCK_MESSAGE *block_chain_msg = malloc(MQ_MAX_MSG_SIZE);
    block_chain_msg->block = malloc(sizeof(BLOCK_T));
    memcpy(block_chain_msg->block, block_chain_head->block, sizeof(BLOCK_T));

    for (;;) {
        mq_getattr(connection_mq, &mqAttr);
        while (mqAttr.mq_curmsgs > 0) {

            CONNECTION_REQUEST_MESSAGE *rec_msg = malloc(MQ_MAX_MSG_SIZE);
            mq_receive(connection_mq, (char *) rec_msg, MQ_MAX_MSG_SIZE, NULL);

            unsigned int miner_id = rec_msg->id;
            char miner_que_name[CHAR_SIZE];
            strcpy(miner_que_name, rec_msg->que_name);

            printf("Server received connection request from miner id %d, queue name %s\n", miner_id, miner_que_name);

            miners_mq[numberOfConnections] = mq_open(miner_que_name, O_WRONLY);
            //print_block(((BLOCK_MESSAGE *) block_chain_msg->data)->block);

            mq_send(miners_mq[numberOfConnections], (char *) block_chain_msg, MQ_MAX_MSG_SIZE, 0);
            mq_getattr(miners_mq[numberOfConnections], &mqAttr);
            printf("Server send massege to miner %d\n", miner_id);

            printf("Miners sent new block request Q: remaining %ld messages in queue %s\n", mqAttr.mq_curmsgs, miner_que_name);
            numberOfConnections++;
            free(rec_msg);
        }

//        mq_getattr(newBlock_mq, &mqAttr);
//        if (mqAttr.mq_curmsgs > 0) {
//            printf("Server get BLOCK message\n");
//            MSG_T *msg = malloc(MQ_MAX_MSG_SIZE);
//            mq_receive(newBlock_mq, (char *) msg, MQ_MAX_MSG_SIZE, NULL);
//            BLOCK_T *minerBlockReceived = ((BLOCK_MESSAGE *) msg->data)->block;
//            print_block(minerBlockReceived);
//
//        }


        // for(int i = 0; i < numberOfConnections; i++){

        //     mq_getattr(miners_mq[i], &mqMinersAttr);

        //     if(mqMinersAttr.mq_curmsgs > 0) {
        //         mq_receive(miners_mq[i], (char *) msg, MQ_MAX_MSG_SIZE, NULL);
        //         printf("Server get BLOCK message\n");
        //         BLOCK_T *minerBlockReceived = ((BLOCK_MESSAGE*)msg->data)->block;
        //         print_block(minerBlockReceived);
        //         printf("MINER QUE ID(#%u): remaining %ld messages in queue\n", i, mqMinersAttr.mq_curmsgs);


        //     }

        // }
    }
}

void createMinersMessageQues() {
    for (int i = 0; i < NUM_OF_MINER; i++) {
        mqd_t mq;
        struct mq_attr attr = {0};
        char que_name[CHAR_SIZE];

        /* initialize the queue attributes */
        attr.mq_maxmsg = MQ_MAX_SIZE;
        attr.mq_msgsize = MQ_MAX_MSG_SIZE;

        /* create the message queue and close(not delete) it immidiatly as it will be used only by children */
        sprintf(que_name, MQ_MINERS_TEMPLATE_NAME, i);
        mq_unlink(que_name); // delete first if already exists, this requires sudo privilege
        mq = mq_open(que_name, O_CREAT, S_IRWXU | S_IRWXG, &attr);
    }
}

void createServerBlocksMessageQues() {
    mqd_t mq_server;
    struct mq_attr server_attr = {0};

    /* initialize the queue attributes */
    server_attr.mq_maxmsg = MQ_MAX_SIZE;
    server_attr.mq_msgsize = MQ_MAX_MSG_SIZE;

    /* create the message queue and close(not delete) it immidiatly as it will be used only by children */
    //mq_unlink(MQ_SERVER_NAME); // delete first if already exists, this requires sudo privilege
    //mq_server = mq_open(MQ_SERVER_NAME, O_CREAT, S_IRWXU | S_IRWXG, &server_attr);
}

//void *server(void *pVoid)
//{
//    printf("Server: thread started\n");
//    int policy = -1;
//    struct sched_param sp = {0};
//    pthread_getschedparam(pthread_self(), &policy, &sp);
//    printf("Server priority %d, policy %s\n", sp.sched_priority, POLICY_STR(policy));
//    pthread_mutex_lock(&chain_lock);
//
//    generateInitBlock();
//
//    pthread_mutex_unlock(&chain_lock);
//
//    BLOCK_T *serverBlock = NULL;
//
//    for (;;)
//    {
//        pthread_mutex_lock(&chain_lock);
//
//        if (serverBlock == NULL)
//        {
//            pthread_cond_wait(&new_block_cond, &chain_lock);
//        }
//
//        serverBlock = newBlock;
//
//        if (isLegalBlock(serverBlock))
//        {
//            print_block(serverBlock);
//            push(&block_chain_head, &serverBlock);
//        }
//
//        newBlock = NULL;
//        serverBlock = NULL;
//
//        pthread_mutex_unlock(&chain_lock);
//    }
//
//    //delete_list();
//}
//
void generateInitBlock() {

    NODE_T *head_node_list = malloc(sizeof(NODE_T));
    BLOCK_T *first_block = malloc(sizeof(BLOCK_T));
    first_block->height = 0;
    first_block->timestamp = (int) time(NULL); // current time in seconds
    first_block->relayed_by = -1;             // server id
    first_block->nonce = 0;                   // dummy nonce
    first_block->prev_hash = 0;               // dummy prev hash
    first_block->hash = 0;                    // dummy hash
    first_block->difficulty = NUM_OF_ZERO;

    head_node_list->prev = NULL;
    head_node_list->block = first_block;
    block_chain_head = head_node_list; // init head of list
}

//int isLegalBlock(BLOCK_T *serverBlock)
//{
//    unsigned long hash = generateHashFromBlock(serverBlock);
//    if (hash != serverBlock->hash)
//
//    {
//        printf("Server: Wrong hash for block #%d by miner #%d, received %08x but calculate %08x\n", serverBlock->height,
//               serverBlock->relayed_by, (unsigned int)serverBlock->hash, (unsigned int)hash);
//        return 0;
//    }
//
//    if ((serverBlock->hash & mask) != 0)
//    {
//        printf("Server: Wrong hash difficulty for block #%d by miner #%d, received %08x \n", serverBlock->height,
//               serverBlock->relayed_by, (unsigned int)serverBlock->hash);
//        return 0;
//    }
//    int height = block_chain_head->block->height;
//    if (height + 1 != serverBlock->height)
//    {
//        printf("Server: Wrong height for block by miner #%d, received %d but supposed to be %d\n", serverBlock->relayed_by,
//               serverBlock->height, height + 1);
//        return 0;
//    }
//
//    return 1;
//}
