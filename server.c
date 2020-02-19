#include "server.h"

void main()
{
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
    generateMask();

    struct mq_attr mqAttr = {0};
    for (;;)
    {
        mq_getattr(connection_mq, &mqAttr);
        //printf("Server connections mq message %ld\n", mqAttr.mq_curmsgs);

        while (mqAttr.mq_curmsgs > 0)
        {

            CONNECTION_REQUEST_MESSAGE *rec_msg = (CONNECTION_REQUEST_MESSAGE *)malloc(sizeof(CONNECTION_REQUEST_MESSAGE));
            mq_receive(connection_mq, (char *)rec_msg, MQ_MAX_MSG_SIZE, NULL);

            unsigned int miner_id = rec_msg->id;
            char miner_que_name[CHAR_SIZE];
            strcpy(miner_que_name, rec_msg->que_name);

            printf("Server received connection request from miner id %d, queue name %s\n", miner_id, miner_que_name);

            free(rec_msg);

            miners_mq[numberOfConnections] = mq_open(miner_que_name, O_WRONLY);

            //print_block(blockToSend);
            sendBlock(&(miners_mq[numberOfConnections]), block_chain_head->block, "server block to send");

            mq_getattr(miners_mq[numberOfConnections], &mqAttr);
            printf("Server send massege to miner %d\n", miner_id);

            numberOfConnections++;
            mq_getattr(connection_mq, &mqAttr);
        }

        mq_getattr(newBlock_mq, &mqAttr);
        if (mqAttr.mq_curmsgs > 0)
        {
            printf("Server get BLOCK message\n");
            checkAndUpdateBlockChainHead(&newBlock_mq);

            for (int i = 0; i < numberOfConnections; i++)
            {
                mq_getattr(miners_mq[i], &mqAttr);
                printf("Server: size of miners_mq_%d: %ld\n", i, mqAttr.mq_curmsgs);
                sendBlock(&(miners_mq[i]), block_chain_head->block, "server block to send");
            }
        }
    }
}

void *checkAndUpdateBlockChainHead(mqd_t *newBlock_mq)
{
    BLOCK_T *minded_block = malloc(sizeof(BLOCK_T));
    // TODO sucss to malloc here
    receiveBlock(newBlock_mq, minded_block, "server in receiveBlock() ");

    if (isLegalBlock(minded_block))
    {
        NODE_T *new_head = (NODE_T *)malloc(sizeof(NODE_T));
        push(&block_chain_head, minded_block, new_head);
        print_block_with_message(block_chain_head->block, "SERVER CHANGED HEAD WITH THIS BLOCK: ");
    }

    print_block_with_message(block_chain_head->block, "SERVER WILL SEND THIS BLOCK: ");
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
void generateInitBlock()
{

    NODE_T *head_node_list = malloc(sizeof(NODE_T));
    BLOCK_T *first_block = malloc(sizeof(BLOCK_T));
    first_block->height = 0;
    first_block->timestamp = (int)time(NULL); // current time in seconds
    first_block->relayed_by = -1;             // server id
    first_block->nonce = 0;                   // dummy nonce
    first_block->prev_hash = 0;               // dummy prev hash
    first_block->hash = 0;                    // dummy hash
    first_block->difficulty = NUM_OF_ZERO;

    head_node_list->prev = NULL;
    head_node_list->block = first_block;
    block_chain_head = head_node_list; // init head of list
}

int isLegalBlock(BLOCK_T *serverBlock)
{
    unsigned long hash = generateHashFromBlock(serverBlock);
    if (hash != serverBlock->hash)

    {
        printf("Server: Wrong hash for block #%d by miner #%d, received %08x but calculate %08x\n", serverBlock->height,
               serverBlock->relayed_by, (unsigned int)serverBlock->hash, (unsigned int)hash);
        return 0;
    }

    if ((serverBlock->hash & mask) != 0)
    {
        printf("Server: Wrong hash difficulty for block #%d by miner #%d, received %08x \n", serverBlock->height,
               serverBlock->relayed_by, (unsigned int)serverBlock->hash);
        return 0;
    }
    int height = block_chain_head->block->height;
    if (height + 1 != serverBlock->height)
    {
        printf("Server: Wrong height for block by miner #%d, received %d but supposed to be %d\n", serverBlock->relayed_by,
               serverBlock->height, height + 1);
        return 0;
    }

    return 1;
}
