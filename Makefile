.SUFFIXES: .c .o
CC = gcc
EXEC = fge
CCFLAGS = -O2 -Wall

${EXEC}: ${EXEC}.c
	${CC} ${CCFLAGS} ${EXEC}.c -lm -o ${EXEC}

run: ${EXEC}
	./${EXEC}
    
clean:
	rm -f ${EXEC}
