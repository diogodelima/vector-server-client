CC = gcc

CFLAGS += -Wall 
CFLAGS += -g
LDLIBS += -lm

SUBMIT_FILENAME = ex1.tar.gz

BINS = vector_stat_server vector_stat_client

all: $(BINS)

vector_stat_client: sockets.o errors.o vector_stat_thread.o
vector_stat_server: vector_stat_server.o sockets.o errors.o vector_stat_thread.o vector_stat_proc.o
vector_stat_server.o: vector_stat_server.c
errors.o: errors.c errors.h
vector_stat_proc.o: vector_stat_proc.c vector_stat_proc.h
vector_stat_thread.o: vector_stat_thread.c vector_stat_thread.h
sockets.o: sockets.h sockets.c

clean:
	$(RM) $(BINS) *.o $(SUBMIT_FILENAME)

submit: clean
	tar -czvf $(SUBMIT_FILENAME) *
