#include "miner.h"

void main(int argc, char **argv)
{
    char miner_que_name[CHAR_SIZE] = "/miner_q_";
    struct mq_attr mqInitAttr = {0};
    struct mq_attr mqNewBlocktAttr = {0};
    struct mq_attr mqMinertAttr = {0};
    BLOCK_T minerBlock;
    CONNECTION_REQUEST_MESSAGE connection_msg;
    int isMinerBlockGenerate = 0;

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

    connection_msg.id = (unsigned int)miner_id;
    strcpy(connection_msg.que_name, miner_que_name);

    mq_send(connection_mq, (char *)&connection_msg, MQ_MAX_MSG_SIZE, 0);
    generateMask();

    for (;;)
    {
        /* Check if there is place in the Q, to send generated block to server */
        mq_getattr(newBlock_mq, &mqNewBlocktAttr);

        printf("I'm Miner\n");
        if (mqNewBlocktAttr.mq_curmsgs == MQ_MAX_SIZE)
        {
            printf("Queue(%d) reached max number of messages(%ld)\n", newBlock_mq, mqNewBlocktAttr.mq_maxmsg);
        }
        else
        {
            /* Check if server sent the latest block in chain */

            mq_getattr(miner_mq, &mqMinertAttr);
            if (mqMinertAttr.mq_curmsgs > 0)
            {
                BLOCK_T *received_block = (BLOCK_T *)malloc(sizeof(BLOCK_T));

                mq_receive(miner_mq, (char *)received_block, MQ_MAX_MSG_SIZE, NULL);
                mq_getattr(miner_mq, &mqMinertAttr);
                printf("Miner %d: after mq_receive: %ld\n", miner_id, mqMinertAttr.mq_curmsgs);
                printf("Miner %d: Received message from server\n", miner_id);
                generateMinerBlock(&minerBlock, received_block, miner_id); //get the new block
                print_block(&minerBlock);
                sleep(10);
                isMinerBlockGenerate = 1;
                free(received_block);
            }

            if (isMinerBlockGenerate == 1)
            {
                if (((minerBlock.hash & mask) == 0))
                {
                    BLOCK_T *newBlock = malloc(sizeof(BLOCK_T));
                    memcpy(newBlock, &minerBlock, sizeof(BLOCK_T));
                    //newBlock = minerBlock;
                    printf("Miner #%d: Mined a new block #%d, with the hash 0x%08x\n", minerBlock.relayed_by,
                           minerBlock.height,
                           (unsigned int)minerBlock.hash);
                    printf("#Before ques: %ld\n", mqNewBlocktAttr.mq_curmsgs);
                    mq_send(newBlock_mq, (char *)newBlock, MQ_MAX_MSG_SIZE, 0);
                    mq_getattr(newBlock_mq, &mqNewBlocktAttr);
                    printf("#After ques: %ld\n", mqNewBlocktAttr.mq_curmsgs);

                    free(newBlock);

                    // // waiting to new approved block by the server
                    //  BLOCK_T *newBlock2 = malloc(sizeof(BLOCK_T));
                    //  mq_receive(miner_mq, (char *) newBlock2,MQ_MAX_MSG_SIZE, NULL);
                    //  printf("heyyy\n");
                    // BLOCK_T *block = newBlock2;
                    //  printf("****after height(%d), timestamp(%d), hash(0x%08x), prev_hash(0x%08x), difficulty(%d), nonce(%d)\n",
                    //  block->height, block->timestamp, (unsigned int)block->hash, (unsigned int)block->prev_hash,
                    //   block->difficulty, block->nonce);
                    isMinerBlockGenerate = 0;
                    printf("isMinerBlockGenerate is 0 \n");
                    sleep(10);
                }
                else
                {
                    updateMinerBlock(&minerBlock);
                    print_block(&minerBlock);
                }
            }
        }
    }
}

void updateMinerBlock(BLOCK_T *minerBlock)
{
    minerBlock->nonce = rand();
    minerBlock->timestamp = (int)time(NULL); // current time
    minerBlock->hash = generateHashFromBlock(minerBlock);
}

void generateMinerBlock(BLOCK_T *minerBlock, BLOCK_T *blockChain, int relayed_by)
{
    minerBlock->height = blockChain->height + 1;
    minerBlock->timestamp = (int)time(NULL);  // current time in seconds
    minerBlock->relayed_by = relayed_by;      // server id
    minerBlock->nonce = 0;                    // dummy nonce
    minerBlock->prev_hash = blockChain->hash; // dummy prev hash
    minerBlock->hash = -1;                    // dummy hash
    minerBlock->difficulty = NUM_OF_ZERO;
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
