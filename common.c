#include "common.h"

unsigned long generateHashFromBlock(BLOCK_T *block)
{
    char block_to_string[MAX_STRING_SIZE];
    sprintf(block_to_string, "%d,%d,%d,%d,%d,%lu",
            block->timestamp, block->nonce, block->height,
            block->relayed_by, block->difficulty, block->prev_hash);
    return crc32(0, (const void *)block_to_string, (uInt)strlen(block_to_string));
}

void generateMask()
{
    int gen_mask = 0xffffffff;
    mask = gen_mask << NUM_OF_ZERO;
}

void delete_list()
{
    node_t *node;
    while (block_chain_head)
    {
        free(block_chain_head->block);
        node = block_chain_head;
        block_chain_head = block_chain_head->prev;
        free(node);
    }
}

void print_block(BLOCK_T *block)
{
    printf("height(%d), timestamp(%d), hash(0x%08x), prev_hash(0x%08x), difficulty(%d), nonce(%d)\n",
           block->height, block->timestamp, (unsigned int)block->hash, (unsigned int)block->prev_hash,
           block->difficulty, block->nonce);
}

void print_list()
{
    node_t *current = block_chain_head->prev;
    print_block(block_chain_head->block);

    while (current != NULL)
    {
        printf("      |\n");
        printf("      |\n");
        printf("      V\n");
        BLOCK_T *temp_block = current->block;
        print_block(temp_block);

        current = current->prev;
    }
}
