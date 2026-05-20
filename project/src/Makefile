PROG=proj

SRCS= \
	application.c \
	game.c \
	./inputHandler.c \
	./boxCollider.c \
	./rigidBody.c \
	./arena.c \
	cursorHandler.c \
	animatedSprite.c \
	./vec2d.c \
	./player.c \
	./utils.c \
	./playerController.c \
	./playerIdle.c \
	./playerModel.c \
	./playerState.c \
	./playerRunning.c \

OBJS = ${SRCS:.c=.o}

CPPFLAGS += -Iinclude -DLAB3
CFLAGS += -Wall -Wextra -Werror -pedantic -std=c11

DPADD += ${LIBLCF}
LDADD += -llcf -L.
LDADD += -L../../lab2 -ltimer
LDADD += -L../../lab3 -lkbc
LDADD += -L../../lab4 -lmouse
LDADD += -L../../lab5 -lterminal 
LDADD += -L../../lab5 -lspriteXPM

all:
	mkdir -p ../build
	mv $(OBJS) ../build

.include <minix.lcom.mk>
