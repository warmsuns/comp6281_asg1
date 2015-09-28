INCLPATH +=/usr/include
CC = gcc
CFLAGS  = -g -Wall -std=c11

OBJECTS = utility.o log.o find.o

utility: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o utility
log.o: log.c log.h
	$(CC) $(CFLAGS) -o log.o -c log.c -I$(INCLPATH)
find.o: find.c find.h
	$(CC) $(CFLAGS) -o find.o -c find.c -I$(INCLPATH)
utility.o: utility.c
	$(CC) $(CFLAGS) -o utility.o -c utility.c -I$(INCLPATH)
clean:
	$(RM) *.o

