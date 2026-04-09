# name of the program (Minix service)
PROG=lab3

# source code files to be compiled
.PATH: ${.CURDIR}/.
SRCS = lab3.c kbd.c 

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB3 -I../lab2/

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIB_NAME}              # Let make track the versions of the new target
LDADD += -llcf -L.                          # Tell the compiler to look in the current directory for libraries
LDADD += -lkbd                              #   and link the library named libtimers.a

LIB_NAME = libkbd.a                         # The name filename for your library
LIB_SRCS = kbd.c utils.c                 # The source files to include
LIB_OBJS = ${LIB_SRCS:.c=.o}                # The rule used to convert your .c source files in .o object files
CLEANFILES += ${LIB_NAME} ${LIB_OBJS}       # Add the library file and object files to the files to be cleaned

all: lib                                    # Instruct make run the lib compilation target before building the lab2 program
lib: ${LIB_OBJS} ; ar rcs ${LIB_NAME} ${LIB_OBJS} # Define and package the library

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
