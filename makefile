CC	= g++
CFLAGS	= -Wall -I.
TARGET	= wat
CFILES  = wat.cc markov.o markov_dict.o markov_prefix.o

all: $(CFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES)

markov: markov.cc markov.h
	$(CC) $(CFLAGS) -c markov.cc

markov_dict: markov_dict.cc markov_dict.h
	$(CC) $(CFLAGS) -c markov_dict.cc

markov_prefix: markov_prefix.cc markov_prefix.h
	$(CC) $(CFLAGS) -c markov_prefix.cc

clean:
	rm -f $(TARGET) *.o
