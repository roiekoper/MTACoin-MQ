#include "miner.h"

void main(int argc, char **argv) {
    char miner_que_name[CHAR_SIZE] = "/miner_q_";
    struct mq_attr mqInitAttr = {0};
    struct mq_attr mqNewBlocktAttr = {0};
    BLOCK_T *minerBlock = NULL;

    /* initialize the queue attributes */
    mqInitAttr.mq_maxmsg = MQ_MAX_SIZE;
    mqInitAttr.mq_msgsize = MQ_MAX_MSG_SIZE;

    // ----------------------
    // get miner id from args
    int miner_id;
    sscanf(argv[1], "%d", &miner_id);
    strcat(miner_que_name, argv[1]);
    printf("Miner strlen(miner_que_name) = %ld %s\n", strlen(miner_que_name), miner_que_name);
    // ----------------------


    // generate all mqs
    mqd_t newBlock_mq = mq_open(MQ_NEW_BLOCK_NAME, O_WRONLY);
    mqd_t connection_mq = mq_open(MQ_CONNECTION_REQUEST_NAME, O_WRONLY);

    mq_unlink(miner_que_name);
    mqd_t miner_mq = mq_open(miner_que_name, O_CREAT, S_IRWXU | S_IRWXG, &mqInitAttr);

    CONNECTION_REQUEST_MESSAGE *connection_msg = malloc(MQ_MAX_MSG_SIZE);
    connection_msg->id = (unsigned int)miner_id;
    strcpy(connection_msg->que_name, miner_que_name);

    mq_send(connection_mq, (char *) connection_msg, MQ_MAX_MSG_SIZE, 0);
    free(connection_msg);

    for (int i = 0; i < 10; i++) {
        /* Check if there is place in the Q, to send generated block to server */
        mq_getattr(newBlock_mq, &mqNewBlocktAttr);

        if (mqNewBlocktAttr.mq_curmsgs == MQ_MAX_SIZE) {
            printf("Queue(%d) reached max number of messages(%ld)\n", newBlock_mq, mqNewBlocktAttr.mq_maxmsg);
        } else {
            /* Check if server sent the latest block in chain */

            mq_getattr(miner_mq, &mqNewBlocktAttr);
            //printf("%s status: %ld\n",miner_que_name, mqNewBlocktAttr.mq_curmsgs);
            if (mqNewBlocktAttr.mq_curmsgs > 0) {
                BLOCK_T *received_block = (BLOCK_T *)malloc(sizeof(BLOCK_T));
                mq_receive(miner_mq, (char *) received_block, MQ_MAX_MSG_SIZE, NULL);
                mq_getattr(miner_mq, &mqNewBlocktAttr);
                printf("Miner %d: after mq_receive: %ld\n", miner_id, mqNewBlocktAttr.mq_curmsgs);
                printf("Miner %d: Received message from server\n", miner_id);
                print_block(received_block);

                print_block(received_block);
                minerBlock = generateMinerBlock(received_block, miner_id); //get the new block
            }

            // minerBlock = generateMinerBlock(getpid()); //get the new block

            // if (minerBlock && ((minerBlock->hash & mask) == 0)) {
            //     MSG_T *newBlock_msg = malloc(MQ_MAX_MSG_SIZE);
            //     //newBlock = minerBlock;
            //     printf("Miner #%d: Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by,
            //             minerBlock->height,
            //             (unsigned int) minerBlock->hash);
            //     newBlock_msg = malloc(sizeof(MSG_T));
            //     newBlock_msg->type = BLOCK;
            //     ((BLOCK_MESSAGE *) newBlock_msg->data)->block = minerBlock;
            //     mq_send(newBlock_mq, (char *) newBlock_msg, MQ_MAX_MSG_SIZE, 0);

            //     free(newBlock_msg);
            // } else {
            //     updateMinerBlock(minerBlock);
            // }
        }
    }
}


void updateMinerBlock(BLOCK_T *minerBlock) {
    minerBlock->nonce = rand();
    minerBlock->timestamp = (int) time(NULL); // current time
    minerBlock->hash = generateHashFromBlock(minerBlock);
}

BLOCK_T *generateMinerBlock(BLOCK_T *blockChain, int relayed_by) {
    BLOCK_T *new_block = malloc(sizeof(BLOCK_T));
    new_block->height = blockChain->height + 1;
    new_block->timestamp = (int) time(NULL);               // current time in seconds
    new_block->relayed_by = relayed_by;                   // server id
    new_block->nonce = 0;                                 // dummy nonce
    new_block->prev_hash = blockChain->hash; // dummy prev hash
    new_block->hash = -1;                                 // dummy hash
    new_block->difficulty = NUM_OF_ZERO;

    return new_block;
}


//void *miner(int * )
//{
//    BLOCK_T *minerBlock = NULL;
//    int policy = -1;
//    struct sched_param sp = {0};
//
//    pthread_getschedparam(pthread_self(), &policy, &sp);
//    printf("Miner #%d with priority %d, policy %s\n", *thread_id, sp.sched_priority, POLICY_STR(policy));
//
//    while (block_chain_head == NULL)
//    {
//    };
//
//    for (;;)
//    {
//        if (minerBlock == NULL || minerBlock->prev_hash != block_chain_head->block->hash)
//        {
//            pthread_mutex_lock(&chain_lock);
//            printf("Miner %d: generate miner block\n", *thread_id);
//            minerBlock = generateMinerBlock(*thread_id); //get the new block
//            pthread_mutex_unlock(&chain_lock);
//        }
//
//        if ((minerBlock->hash & mask) == 0)
//        {
//            pthread_mutex_lock(&chain_lock);
//            if (newBlock == NULL)
//            {
//                newBlock = minerBlock;
//                printf("Miner #%d: Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by,
//                       minerBlock->height,
//                       (unsigned int)minerBlock->hash);
//
//                pthread_cond_signal(&new_block_cond); //sent server signal there is a new block to add
//            }
//            pthread_mutex_unlock(&chain_lock);
//        }
//        else
//        {
//            updateMinerBlock(minerBlock);
//        }
//    }
//}
//
//void *badMiner(int *thread_id)
//{
//    BLOCK_T *minerBlock = NULL;
//    int policy = -1;
//    struct sched_param sp = {0};
//    int bad_mask = 0xff000000;
//
//    pthread_getschedparam(pthread_self(), &policy, &sp);
//    printf("Miner #%d (bad mainer) with priority %d, policy %s\n", *thread_id, sp.sched_priority, POLICY_STR(policy));
//
//    while (block_chain_head == NULL)
//    {
//    };
//
//    for (;;)
//    {
//        if (minerBlock == NULL || minerBlock->prev_hash != block_chain_head->block->hash)
//        {
//            pthread_mutex_lock(&chain_lock);
//            printf("Miner %d(bad mainer): generate miner block\n", *thread_id);
//            minerBlock = generateMinerBlock(*thread_id); //get the new block
//            pthread_mutex_unlock(&chain_lock);
//            sleep(0.1);
//        }
//
//        if ((minerBlock->hash & bad_mask) == 0)
//        {
//            pthread_mutex_lock(&chain_lock);
//            if (newBlock == NULL)
//            {
//                newBlock = minerBlock;
//                printf("Miner #%d(bad mainer): Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by,
//                       minerBlock->height,
//                       (unsigned int)minerBlock->hash);
//
//                pthread_cond_signal(&new_block_cond); //sent server signal there is a new block to add
//            }
//            pthread_mutex_unlock(&chain_lock);
//        }
//        else
//        {
//            updateMinerBlock(minerBlock);
//        }
//    }
//}
//
