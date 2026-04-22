# name of the program (Minix service) - Lab 3
PROG=lab5

# source code files to be compiled
SRCS = lab5.c graphics.c

# additional compilation flags (required for Lab 3)
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -DLAB5
CFLAGS += -pedantic

# Headers from lab2 (timer.h, utils.h, i8254.h); build lab2 first: make -C ../lab2 lib
CFLAGS += -I../lab2

# Link against libtimer.a from lab2 (contains timer.o and utils.o)
DPADD += ${LIBLCF}
LDADD += -L../lab2 -ltimer -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
