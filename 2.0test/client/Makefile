SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CPPFLAGS:=
CFLAGS:= -g -Wall -I /usr/include/mysql/
$(ELF):$(OBJS)
	gcc $^ -o $@ -lpthread -lmysqlclient
.PHONY:clean
clean:
	rm -rf $(ELF) $(OBJS)
