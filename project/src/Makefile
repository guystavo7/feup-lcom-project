PROG=proj

SRCS= \
	application.c \
	game.c \

OBJS = ${SRCS:.c=.o}

CPPFLAGS += -Iinclude -DLAB3
CFLAGS += -Wall -Wextra -Werror -pedantic -std=c11

DPADD += ${LIBLCF}
LDADD += -llcf -L.
LDADD += -L../../lab2 -ltimer
LDADD += -L../../lab4 -lmouse

all:
	mkdir -p ../build
	mv $(OBJS) ../build

.include <minix.lcom.mk>
