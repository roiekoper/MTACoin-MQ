#include "lib/entities.h"
//#include <sys/fcntl.h>

int main() {
    pthread_t wpid, rpid[NUM_OF_MINER];

    /* create writer process */
    wpid = fork();
    if (wpid == 0) //Writer
    {
        char *argv[] = {"./build/server.out", 0};
        execv("./build/server.out", argv);
    }

    /* Create reader process */
    for (int i = 0; i < NUM_OF_MINER; i++) {
        rpid[i] = fork();
        if (rpid[i] == 0) //Reader
        {
            char miner_index[5];
            sprintf(miner_index, "%d", i);

            char *argv[] = {"./build/miner.out", miner_index, NULL};
            execv("./build/miner.out", argv);
        }
    }

    // Do nothing, only parent process should get here
    pause();

    return 0;
}
