#include "miner.h"

void main(int argc, char **argv) {
    char miner_que_name[CHAR_SIZE] = "/miner_q_";
    struct mq_attr mqInitAttr = {0};
    struct mq_attr mqNewBlocktAttr = {0};
    struct mq_attr mqMinertAttr = {0};
    BLOCK_T minerBlock;
    CONNECTION_REQUEST_MESSAGE connection_msg;
    int isMinerBlockGenerate = 0;
    int isFirstBlockFromServer = 1;

    // initialize the queue attributes
    mqInitAttr.mq_maxmsg = MQ_MAX_SIZE;
    mqInitAttr.mq_msgsize = MQ_MAX_MSG_SIZE;

    // get miner id from args
    int miner_id = createMinerQueueName(argv, miner_que_name);

    // generate all mqs
    mqd_t newBlock_mq = mq_open(MQ_NEW_BLOCK_NAME, O_WRONLY);
    mqd_t connection_mq = mq_open(MQ_CONNECTION_REQUEST_NAME, O_WRONLY);

    mq_unlink(miner_que_name);
    mqd_t miner_mq = mq_open(miner_que_name, O_CREAT, S_IRWXU | S_IRWXG, &mqInitAttr);

    connection_msg.id = (unsigned int) miner_id;
    strcpy(connection_msg.que_name, miner_que_name);

    // send connection request
    mq_send(connection_mq, (char *) &connection_msg, MQ_MAX_MSG_SIZE, 0);
    printf("Miner %d sent connection request on %s\n", miner_id, miner_que_name);

    generateMask();

    for (;;) {
        /* Check if there is place in the Q, to send generated block to server */
        mq_getattr(newBlock_mq, &mqNewBlocktAttr);

        if (mqNewBlocktAttr.mq_curmsgs == MQ_MAX_SIZE) {
            printf("Queue(%d) reached max number of messages(%ld)\n", newBlock_mq, mqNewBlocktAttr.mq_maxmsg);
        } else {
            /* Check if server sent the latest block in chain */
            mq_getattr(miner_mq, &mqMinertAttr);
            if (mqMinertAttr.mq_curmsgs > 0) {
                BLOCK_T *received_block = (BLOCK_T *) malloc(sizeof(BLOCK_T));

                //Get the last MQ from server which is the latest block
                while (mqMinertAttr.mq_curmsgs > 0) {
                    receiveBlock(&miner_mq, received_block);
                    mq_getattr(miner_mq, &mqMinertAttr);
                }

                if (!isMinerBlockGenerate || (minerBlock.prev_hash != received_block->hash)) {
                    printf("Miner %d received %s block: ", miner_id, isFirstBlockFromServer ? "first" : "");
                    print_block(received_block);
                    generateMinerBlock(&minerBlock, received_block, miner_id); //get the new block
                    isMinerBlockGenerate = 1;
                    isFirstBlockFromServer = 0;
                }

                free(received_block);
            }

            if (isMinerBlockGenerate) {
                minerBlockGenerated(&mqNewBlocktAttr, &minerBlock, &isMinerBlockGenerate, &newBlock_mq);
            }
        }
    }
}

int createMinerQueueName(char *const *argv, const char *miner_que_name) {
    int miner_id;
    sscanf(argv[1], "%d", &miner_id);
    strcat(miner_que_name, argv[1]);
    printf("Miner id = %d, queue name = %s\n", miner_id, miner_que_name);
    return miner_id;
}

void minerBlockGenerated(struct mq_attr *mqNewBlocktAttr,
                         BLOCK_T *minerBlock,
                         int *isMinerBlockGenerate,
                         mqd_t *newBlock_mq) {
    if ((((*minerBlock).hash & mask) == 0)) {
        printf("Miner %d: Mined a new block #%d, with the hash 0x%08x, difficulty %d\n", (*minerBlock).relayed_by,
               (*minerBlock).height,
               (unsigned int) (*minerBlock).hash, (*minerBlock).difficulty);
        sendBlock(newBlock_mq, minerBlock);

        mq_getattr((*newBlock_mq), mqNewBlocktAttr);

        (*isMinerBlockGenerate) = 0;
    } else {
        updateMinerBlock(minerBlock);
    }
}

void updateMinerBlock(BLOCK_T *minerBlock) {
    minerBlock->nonce = rand();
    minerBlock->timestamp = (int) time(NULL); // current time
    minerBlock->hash = generateHashFromBlock(minerBlock);
}

void generateMinerBlock(BLOCK_T *minerBlock, BLOCK_T *blockChain, int relayed_by) {
    minerBlock->height = blockChain->height + 1;
    minerBlock->timestamp = (int) time(NULL);  // current time in seconds
    minerBlock->relayed_by = relayed_by;      // server id
    minerBlock->nonce = 0;                    // dummy nonce
    minerBlock->prev_hash = blockChain->hash; // dummy prev hash
    minerBlock->hash = (unsigned long) -1;                    // dummy hash
    minerBlock->difficulty = NUM_OF_ZERO;
}