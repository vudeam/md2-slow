CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -g

BINS = md2
OBJS = main.o utils.o

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

