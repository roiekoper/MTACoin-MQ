#include "miner.h"

void main(int argc, char **argv) {
    int miner_send_request = 0;
    char miner_que_name[CHAR_SIZE];
    struct mq_attr mqAttr = {0};
    BLOCK_T *minerBlock = NULL;
    BLOCK_T *newBlock = NULL;

    int miner_id = (unsigned int) getpid();
    sscanf(argv[1], "%d", &miner_id);

    sprintf(miner_que_name, MQ_MINERS_TEMPLATE_NAME, miner_id);


    sprintf(miner_que_name, MQ_MINERS_TEMPLATE_NAME, miner_id);
    mqd_t newBlock_mq = mq_open(MQ_NEW_BLOCK_NAME, O_WRONLY);
    mqd_t miner_mq = mq_open(miner_que_name, O_RDONLY);
    mqd_t connection_mq = mq_open(MQ_CONNECTION_REQUEST_NAME, O_WRONLY);

    MSG_T* msg = malloc(MQ_MAX_MSG_SIZE);

    printf("Miner %d send CONNECTION_REQUEST\n", miner_id);

    MSG_T *connection_msg;
    connection_msg = malloc(sizeof(MSG_T));
    connection_msg->type = CONNECTION_REQUEST;
    ((CONNECTION_REQUEST_MESSAGE *) connection_msg->data)->id = miner_id;
    ((CONNECTION_REQUEST_MESSAGE *) connection_msg->data)->que_name = miner_que_name;
    mq_send(connection_mq, (char *) connection_msg, MQ_MAX_MSG_SIZE, 0);

    mq_getattr(connection_mq,&mqAttr);

    printf("CONNECTION_REQUEST masseges in Q: %ld\n", mqAttr.mq_curmsgs);

    //free(connection_msg);

    for (;;) {
        /* Check if there is place in the Q, if yes increment send, if no print error and try again */
        mq_getattr(newBlock_mq, &mqAttr);

        if (mqAttr.mq_curmsgs == MQ_MAX_SIZE) {
            printf("Queue(%d) reached max number of messages(%ld)\n", newBlock_mq, mqAttr.mq_maxmsg);
        } else {
            //printf("Miner %d: generate miner block\n", getpid());
            mq_getattr(miner_mq, &mqAttr);
            if(mqAttr.mq_curmsgs > 0) {
                printf("Miner %d: Received massege from server\n",miner_id);
                mq_receive(miner_mq, (char *) msg, MQ_MAX_MSG_SIZE, NULL); 
                BLOCK_T *minerBlockReceived = ((BLOCK_MESSAGE*)msg->data)->block; 
                print_block(minerBlockReceived);
                minerBlock = generateMinerBlock(minerBlockReceived, miner_id); //get the new block
            }

            // minerBlock = generateMinerBlock(getpid()); //get the new block

            if (minerBlock && ((minerBlock->hash & mask) == 0)) {
                MSG_T *newBlock_msg;
                //newBlock = minerBlock;
                printf("Miner #%d: Mined a new block #%d, with the hash 0x%08x\n", minerBlock->relayed_by,
                        minerBlock->height,
                        (unsigned int) minerBlock->hash);
                newBlock_msg = malloc(sizeof(MSG_T));
                newBlock_msg->type = BLOCK;
                ((BLOCK_MESSAGE *) newBlock_msg->data)->block = minerBlock;
                mq_send(newBlock_mq, (char *) newBlock_msg, MQ_MAX_MSG_SIZE, 0);

                free(msg);
            } else {
                updateMinerBlock(minerBlock);
            }
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
