# Makefile for SMS project

TGT:=sms_server
SRC:=               \
    sms_server.c    \
    collect_garb.c  \
    listen_port.c   \
    menu.c          \
    serve_client.c  \
    disconnect.c    \
    zap_servlet.c   \
    list_conn.c     \
    list_stats.c    \
    zero_stats.c    \
    zap_stale.c     \
    get_stale.c
OBJ:=$(SRC:.c=.o)
CC:=gcc
CFLAGS:=-g -Wall
LDLIBS:=-lnsl -lpthread

all: $(TGT)

$(OBJ): sms.h

$(TGT): $(OBJ)

pack: $(SRC) Makefile
	tar cvf B023040011_SP_HW8.tar $(SRC) Makefile sms.h

clean :
	@rm -f $(OBJ) $(TGT) B023040011_SP_HW8.tar
