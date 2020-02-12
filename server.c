#include "server.h"
#include "stdio.h"

void main(){
    float counter = 0;
    struct mq_attr mqAttr = {0};

    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);

    for(;;)
    {
        /* Check if there is place in the Q, if yes increment send, if no print error and try again */
        mq_getattr(mq, &mqAttr);
        if (mqAttr.mq_curmsgs == MQ_MAX_SIZE)
        {
            printf("Queue(%d) reached max number of messages(%ld)\n", mq, mqAttr.mq_maxmsg);
        }
        else
        {
            counter += 0.5;
            MSG_T* msg;

            // Precise allocation should be done according to the type
            // It is also possible to allocate a big buffer(see reader)
            if (counter == (unsigned int)counter)	// if integer use UINT_MSG_DATA_T
            {
                msg = malloc(sizeof(MSG_T) + sizeof(UINT_MSG_DATA_T));
                msg->type = UINT;
                ((UINT_MSG_DATA_T*)msg->data)->uvalue = (unsigned int)counter;
            }
            else 									// if float use FLOAT_MSG_DATA_T
            {
                msg = malloc(sizeof(MSG_T) + sizeof(FLOAT_MSG_DATA_T));
                msg->type = FLOAT;
                ((FLOAT_MSG_DATA_T*)msg->data)->fvalue = counter;
            }

            mq_send(mq, (char*)msg, MQ_MAX_MSG_SIZE, 0);
            printf("Writer: %.1f\n", counter);

            free(msg);
        }
    }
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
//void generateInitBlock()
//{
//
//    node_t *head_node_list = malloc(sizeof(node_t));
//    BLOCK_T *first_block = malloc(sizeof(BLOCK_T));
//    first_block->height = 0;
//    first_block->timestamp = (int)time(NULL); // current time in seconds
//    first_block->relayed_by = -1;             // server id
//    first_block->nonce = 0;                   // dummy nonce
//    first_block->prev_hash = 0;               // dummy prev hash
//    first_block->hash = 0;                    // dummy hash
//    first_block->difficulty = NUM_OF_ZERO;
//
//    head_node_list->prev = NULL;
//    head_node_list->block = first_block;
//    block_chain_head = head_node_list; // init head of list
//}
//
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
