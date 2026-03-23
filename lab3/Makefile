# name of the program (Minix service) - Lab 3
PROG=lab3

# source code files to be compiled
SRCS = lab3.c keyboard.c utils.c 

# additional compilation flags (required for Lab 3)
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -DLAB3
CFLAGS += -pedantic

CFLAGS += -I../lab2                # Specify directories to the compiler to search for header files, libraries

LDADD += -llcf -L../lab2           # Tell the compiler to look in the lab2 directory for libraries
LDADD += -ltimer                   # Link the library named libtimers.a

# list of library dependencies (for Lab 3, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf -L.

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
