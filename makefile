CC = gcc
LS = linkedList
UTILS = utils
MINER = miner
SERVER = server
MAIN = main
BLOCK_CHAIN = blockChain
LIBS = -l$(LS) -l$(UTILS) -l$(MINER) -l$(SERVER) -l$(BLOCK_CHAIN)
CFLAGS = -shared -fPIC
SO_LIBS = -pthread -lz
PWD = $(shell pwd)
LIBS_PATH = -L$(PWD)

LS_SO := $(patsubst %.c,%.so,$(subst ./,,$(shell find . -name "$(LS).c")))
UTILS_SO := $(patsubst %.c,%.so,$(subst ./,,$(shell find . -name "$(UTILS).c")))
MINER_SO := $(patsubst %.c,%.so,$(subst ./,,$(shell find . -name "$(MINER).c")))
SERVER_SO := $(patsubst %.c,%.so,$(subst ./,,$(shell find . -name "$(SERVER).c")))
BLOCK_CHAIN_SO := $(patsubst %.c,%.so,$(subst ./,,$(shell find . -name "$(BLOCK_CHAIN).c")))
MAIN_O := $(patsubst %.c,%.o,$(subst ./,,$(shell find . -name "$(MAIN).c")))

all: $(LS_SO) $(UTILS_SO)
    @echo SRCS: $(SRCS)
    @echo OBJS: $(OBJS)

print:
	@echo PWD: $(PWD)
	@echo LS_SO: $(LS_SO)
	@echo UTILS_SO: $(UTILS_SO)
	@echo MINER_SO: $(MINER_SO)
	@echo SERVER_SO: $(SERVER_SO)
	@echo BLOCK_CHAIN_SO: $(BLOCK_CHAIN_SO)
	@echo MAIN_O: $(MAIN_O)


$(LS).so: $(LS).c
	$(CC) $(CFLAGS) $^ -o lib$@
	@echo $($@)
	export LD_LIBRARY_PATH=$(shell pwd)


$(UTILS).so: $(UTILS).c
	#$(CC) -L$(PWD) $^ -o $@ -l$(LS)
	$(CC) $(CFLAGS) $(UTILS).o -o lib$@
    @echo $($@)


clean:
	find . -name "*.o" rm -rf{} \;
	find . -name "*.so" rm -rf{} \;