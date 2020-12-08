INC= -I/usr/X11R6/include
LIB= -L/usr/X11R6/lib
CFLAGS=-Wall -g ${INC} ${LIB}
CC=gcc

ep.o: xwc.h xwc.c ep.c
	$(CC) $(CFLAGS) -c $*.c

ep: ep.o xwc.o
	${CC} ${CFLAGS} -o ep ep.o xwc.o -lm -lX11 -lXpm

xwc.o: xwc.c

clean: 
	rm -f *.o ep *~