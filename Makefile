CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wextra -Wpedantic -O2

BINS = md2
OBJS = main.o utils.o

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

