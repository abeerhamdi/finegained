.SUFFIXES: .c .o
CC = gcc
EXEC = fge
CCFLAGS = -O2 -Wall
LIBS = -lssl -lcrypto

${EXEC}: ${EXEC}.c
	${CC} ${CCFLAGS} ${LIBS} ${EXEC}.c -lm -o ${EXEC}

run: ${EXEC}
	./${EXEC}
    
clean:
	rm -f ${EXEC}
