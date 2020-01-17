CC = gcc
LS = linkedList
COMMON = common
LIBS = -l$(LS) -l$(COMMON) -pthread -lz
CFLAGS = -c -shared -fPIC
PWD = $(shell pwd)
LIBS_PATH = -L$(PWD)


SO_SRCS := $(subst ./,,$(shell find . -name "$(LS).c"))
SO_OBJS := $(patsubst %.c,%.so,$(SO_SRCS))

SRCS := $(subst $(LS).c $(COMMON).c,,$(subst ./,,$(shell find . -maxdepth 1 -name "*.c")))
OBJS := $(patsubst %.c,%.o,$(SRCS))


all: $(SO_OBJS) $(OBJS)
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)

print:
	@echo PWD: $(PWD)
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo SO_SRCS: $(SO_SRCS)
	@echo SO_OBJS: $(SO_OBJS)


%.so: %.c
	$(CC) $(CFLAGS) $^ -o lib$@
	export LD_LIBRARY_PATH=$(shell pwd)


%.o: %.c
	#$(CC) -L$(PWD) $^ -o $@ $(LIBS)
	$(CC) $(LIBS_PATH) $^ -o $@ $(LIBS)


clean:
	find . -name "*.o" rm -rf{} \;
	find . -name "*.so" rm -rf{} \;