.SUFFIXES: .c .o
CC = gcc
EXEC = fge
CCFLAGS = -O2
OBJS = fge.o util.o crypt.o
LIBS = -lssl -lcrypto

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -lm -o ${EXEC} ${OBJS} ${LIBS}

.c.o:
	${CC} ${CCFLAGS} -c $<

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${EXEC} ${OBJS}

fge.o: fge.c fge.h
crypt.o: crypt.c fge.h
util.o: util.c fge.h

