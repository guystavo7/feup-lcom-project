PROG = linkedList

.PATH: .
SRCS = linkedList.c linkedListTest.c
CFLAGS += -pedantic -Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter

LDADD += -llinkedList -L.

LIB_NAME = liblinkedList.a
LIB_SRCS = linkedList.c
LIB_OBJS = linkedList.o
CLEAN_FILES = linkedList.o liblinkedList.a linkedListTest

TEST_NAME = linkedListTest
TEST_SRCS = linkedListTest.c

all: lib test 
lib: ${LIB_OBJS} 
	ar rcs ${LIB_NAME} ${LIB_OBJS}

test: ${TEST_SRCS} lib
	cc ${CFLAGS} -o ${TEST_NAME} ${TEST_SRCS} ${LDADD} 

clean:
	rm ${CLEAN_FILES}
