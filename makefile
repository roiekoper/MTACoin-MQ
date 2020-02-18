CC = gcc
CC_EXPORT = LD_LIBRARY_PATH=$(LIBS_PATH) $(CC)
PWD = $(shell pwd)
LIBS_PATH = $(PWD)/$(SO_DIR)
CFLAGS = -shared -fPIC
SO_LIBS = -lpthread -lz -lrt

 
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
LAUNCHER = launcher


LS_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(LS).c"))))
#LS_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(LS).c")))

UTILS_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(UTILS).c"))))
#UTILS_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(UTILS).c")))

BLOCK_CHAIN_SO := $(addprefix $(SO_DIR)/,$(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(BLOCK_CHAIN).c"))))
#BLOCK_CHAIN_SO := $(patsubst %.c,%.so,$(subst $(DELETE_LIB),,$(shell find . -name "$(BLOCK_CHAIN).c")))

LAUNCHER_O := $(addprefix $(O_DIR)/,$(patsubst %.c,%.out,$(subst ./,,$(shell find . -name "$(LAUNCHER).c"))))
#LAUNCHER_O := $(patsubst %.c,%.o,$(subst ./,,$(shell find . -name "$(LAUNCHER).c")))

SERVER_O := $(addprefix $(O_DIR)/,$(patsubst %.c,%.out,$(subst ./,,$(shell find . -name "$(SERVER).c"))))

MINER_O := $(addprefix $(O_DIR)/,$(patsubst %.c,%.out,$(subst ./,,$(shell find . -name "$(MINER).c"))))

all: $(LS_SO) $(UTILS_SO) $(SERVER_O) $(MINER_O) $(LAUNCHER_O)
    #@echo SRCS: $(SRCS)
    #@echo OBJS: $(OBJS)

print:
	@echo LIBS_PATH: $(LIBS_PATH)
	@echo LS_SO: $(LS_SO)
	@echo UTILS_SO: $(UTILS_SO)
	@echo $(shell $LD_LIBRARY_PATH)
	@echo BLOCK_CHAIN_SO: $(BLOCK_CHAIN_SO)
	@echo MAIN_O: $(MAIN_O)
	@echo SERVER_O: $(SERVER_O)
	@echo MINER_O: $(MINER_O)
	@echo export: export LD_LIBRARY_PATH=$(LIBS_PATH)


run:
	sudo ./$(LAUNCHER_O)

clean:
	rm -r $(O_DIR)

$(SO_DIR)/$(LS).so: $(LIB)/$(LS).c
	#mkdir -p $(O_DIR)
	mkdir -p $(SO_DIR)
	$(CC) $(CFLAGS) $^ -o $(subst $(LS),lib$(LS),$@) $(SO_LIBS)
	export LD_LIBRARY_PATH=$(LIBS_PATH);

$(SO_DIR)/$(UTILS).so: $(LIB)/$(UTILS).c
	$(CC) $(CFLAGS) -L$(LIBS_PATH) $^ -o $(subst $(UTILS),lib$(UTILS),$@) $(SO_LIBS) -l$(LS)

$(SO_DIR)/$(BLOCK_CHAIN).so: $(LIB)/$(BLOCK_CHAIN).c
	$(CC) $(CFLAGS) -L$(LIBS_PATH) $^ -o $(subst $(BLOCK_CHAIN),lib$(BLOCK_CHAIN),$@) -l$(SERVER) -l$(MINER)

$(O_DIR)/$(LAUNCHER).out: $(LAUNCHER).c
	$(CC) -L$(LIBS_PATH) $^ -o $@ $(SO_LIBS)

$(O_DIR)/$(SERVER).out: $(SERVER).c
	$(CC_EXPORT) -L$(LIBS_PATH) $^ -o $@ $(SO_LIBS) -l$(UTILS) -l$(LS)

$(O_DIR)/$(MINER).out: $(MINER).c
	$(CC_EXPORT) -L$(LIBS_PATH) $^ -o $@ $(SO_LIBS) -l$(UTILS) -l$(LS)
