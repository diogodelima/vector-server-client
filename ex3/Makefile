CC = gcc

CFLAGS += -Wall 
CFLAGS += -g
LDLIBS += -lm

SUBMIT_FILENAME = ex3.tar.gz

BINS = vector_stat_thread

all: $(BINS)

vector_stat_thread: vector_stat_thread.o
vector_stat_thread.o: vector_stat_thread.c

clean:
	$(RM) $(BINS) *.o $(SUBMIT_FILENAME)

submit: clean
	tar -czvf $(SUBMIT_FILENAME) *