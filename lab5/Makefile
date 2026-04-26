# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
.PATH: ${.CURDIR}/.
SRCS = lab5.c graphics.c 

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB3 -I../lab2/ -I../lab3/

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIB_NAME}              # Let make track the versions of the new target
LDADD += -llcf -L.                          # Tell the compiler to look in the current directory for libraries
LDADD += -lgraphics -ltimer -lkbd -L../lab2/ -L../lab3/

LIB_NAME = libgraphics.a                         # The name filename for your library
LIB_SRCS = graphics.c #utils.c                 # The source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}                # The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}       # Add the library file and object files to the files to be cleaned

all: lib

lib: ${LIB_OBJS}
	ar rcs ${LIB_NAME} ${LIB_OBJS}

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
