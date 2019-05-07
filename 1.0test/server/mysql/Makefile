SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=file_sys
CC:=gcc
CPPFLAGS:=
CFLAGS:= -g -Wall -I /usr/include/mysql/
$(ELF):$(OBJS)
	gcc $^ -o $@ -lmysqlclient
.PHONY:clean
clean:
	rm -rf $(ELF) $(OBJS)
