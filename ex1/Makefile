CC = gcc

CFLAGS += -Wall 
CFLAGS += -g
LDLIBS += -lm

SUBMIT_FILENAME = ex1.tar.gz

BINS = main

all: $(BINS)

main: main.o
main.o: main.c

clean:
	$(RM) $(BINS) *.o $(SUBMIT_FILENAME)

submit: clean
	tar -czvf $(SUBMIT_FILENAME) *