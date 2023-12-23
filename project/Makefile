CC = gcc
CFLAGS =
LDFLAGS = -lnsl -lpthread

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = chats chatc chats_s chatc_t

all: $(ALL)

chats: chats.o 
	$(CC) -o $@ $< $(LDFLAGS)

chatc: chatc.o 
	$(CC) -o $@ $< $(LDFLAGS)

chats_s: chats_s.o
	$(CC) -o $@ $< $(LDFLAGS)

chatc_t: chatc_t.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL)
