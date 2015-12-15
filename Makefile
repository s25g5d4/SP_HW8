# Makefile for SMS project

TGT = sms_server

all:	$(TGT)

SRC = 				\
	sms_server.c	\
	collect_garb.c	\
	listen_port.c	\
	menu.c			\
	serve_client.c	\
	disconnect.c	\
	zap_servlet.c	\
	list_conn.c		\
	list_stats.c	\
	zero_stats.c	\
	zap_stale.c		\
	get_stale.c

OBJ = $(SRC:.c=.o)

LIBS = -lnsl -lpthread

$(OBJ) : sms.h

$(TGT) : $(OBJ)
	cc -o $(TGT) $(OBJ) $(LIBS)

pack : $(SRC) Makefile progress.md
	tar cvf B023040011_SP_HW8.tar $(SRC) Makefile progress.md

clean :
	@rm -f $(OBJ) $(TGT)
