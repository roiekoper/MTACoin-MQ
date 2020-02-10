CC = gcc
PWD = $(shell pwd)
LIBS_PATH = -L$(PWD)/$(SO_DIR)
CFLAGS = -shared -fPIC
SO_LIBS = -lpthread -lz

 
#folders path
LIB = lib
DELETE_LIB = ./$(LIB)/
O_DIR = build
SO_DIR = $(O_DIR)/so

#files names
LS = linkedList
UTILS = utils
SERVER = server
MINER = miner
BLOCK_CHAIN = blockChain
MAIN = main


LS_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(LS).c"))))
#LS_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(LS).c")))

UTILS_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(UTILS).c"))))
#UTILS_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(UTILS).c")))

SERVER_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(SERVER).c"))))
#SERVER_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(SERVER).c")))

MINER_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(MINER).c"))))
#MINER_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(MINER).c")))

BLOCK_CHAIN_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(BLOCK_CHAIN).c"))))
#BLOCK_CHAIN_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(BLOCK_CHAIN).c")))

MAIN_O := $(addprefix $(O_DIR)/,$(patsubst %.c,%.o,$(subst ./,,$(shell find . -name "$(MAIN).c"))))
#MAIN_O := $(patsubst %.c,%.o,$(subst ./,,$(shell find . -name "$(MAIN).c")))



all: $(LS_SO) $(UTILS_SO) $(SERVER_SO) $(MINER_SO) $(BLOCK_CHAIN_SO) $(MAIN_O)
    #@echo SRCS: $(SRCS)
    #@echo OBJS: $(OBJS)

print:
	#@echo PWD: $(PWD)
	@echo LS_SO: $(LS_SO)
	@echo UTILS_SO: $(UTILS_SO)
	@echo MINER_SO: $(MINER_SO)
	@echo SERVER_SO: $(SERVER_SO)
	@echo BLOCK_CHAIN_SO: $(BLOCK_CHAIN_SO)
	@echo MAIN_O: $(MAIN_O)

run:
	./$(MAIN_O)

$(SO_DIR)/$(LS).so: $(LIB)/$(LS).c
	#mkdir -p $(O_DIR)
	mkdir -p $(SO_DIR)
	$(CC) $(CFLAGS) $^ -o $(subst $(LS),lib$(LS),$@) $(SO_LIBS)
	export LD_LIBRARY_PATH=$(PWD)

$(SO_DIR)/$(UTILS).so: $(LIB)/$(UTILS).c
	$(CC) $(CFLAGS) $(LIBS_PATH) $^ -o $(subst $(UTILS),lib$(UTILS),$@) -lz  -l$(LS)

$(SO_DIR)/$(SERVER).so: $(LIB)/$(SERVER).c
	$(CC) $(CFLAGS) $(LIBS_PATH) $^ -o $(subst $(SERVER),lib$(SERVER),$@) -l$(UTILS) -l$(LS)

$(SO_DIR)/$(MINER).so: $(LIB)/$(MINER).c
	$(CC) $(CFLAGS) $(LIBS_PATH) $^ -o $(subst $(MINER),lib$(MINER),$@) -l$(UTILS) -l$(LS)

$(SO_DIR)/$(BLOCK_CHAIN).so: $(LIB)/$(BLOCK_CHAIN).c
	$(CC) $(CFLAGS) $(LIBS_PATH) $^ -o $(subst $(BLOCK_CHAIN),lib$(BLOCK_CHAIN),$@) -l$(SERVER) -l$(MINER)

$(O_DIR)/$(MAIN).o: $(MAIN).c
	$(CC) -v $(LIBS_PATH) $^ -o $(subst $(MAIN),lib$(MAIN),$@) -l$(BLOCK_CHAIN) -l$(SERVER) -l$(MINER)