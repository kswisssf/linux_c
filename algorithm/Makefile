#!bin/sh

CC =gcc

ifeq ($(DEBUG),0)
	CFLAGS		+= -O2
else
	CFLAGS		+= -O0 -g
endif


BIN = algorithm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.C = %.o)

$(BIN):$(OBJS)
	   $(CC) $(CFLAGS) -o $@ $^ -lcrypt


clean:
	rm -rf $(BIN)
	rm -rf *.o *.d	   

.PHONY: clean
