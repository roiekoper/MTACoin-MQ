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
	#sudo mv $(LIBS_PATH)/* /usr/lib
    #@echo SRCS: $(SRCS)
    #@echo OBJS: $(OBJS)

so: $(LS_SO) $(UTILS_SO)
print:
	@echo LIBS_PATH: $(LIBS_PATH)
	@echo LS_SO: $(LS_SO)
	@echo UTILS_SO: $(UTILS_SO)
	@echo BLOCK_CHAIN_SO: $(BLOCK_CHAIN_SO)
	@echo MAIN_O: $(MAIN_O)
	@echo SERVER_O: $(SERVER_O)
	@echo MINER_O: $(MINER_O)


run:
	sudo ./$(LAUNCHER_O)

clean:
	rm -r $(O_DIR)

$(SO_DIR)/$(LS).so: $(LIB)/$(LS).c
	mkdir -p $(SO_DIR)
	$(CC) $(CFLAGS) $^ -o $(subst $(LS),lib$(LS),$@) $(SO_LIBS)
	sudo mv $(LIBS_PATH)/* /usr/lib
	

$(SO_DIR)/$(UTILS).so: $(LIB)/$(UTILS).c
	$(CC) $(CFLAGS) -L$(LIBS_PATH) $^ -o $(subst $(UTILS),lib$(UTILS),$@) $(SO_LIBS) -l$(LS)

$(O_DIR)/$(LAUNCHER).out: $(LAUNCHER).c
	$(CC) $^ -o $@ $(SO_LIBS)

$(O_DIR)/$(SERVER).out: $(SERVER).c
	$(CC)  $^ -o $@ $(SO_LIBS) -l$(UTILS) -l$(LS)

$(O_DIR)/$(MINER).out: $(MINER).c
	$(CC)  $^ -o $@ $(SO_LIBS) -l$(UTILS) -l$(LS)
