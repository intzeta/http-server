CC = gcc
CCFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wfloat-equal
CCFLAGS += -O2 -g -fno-strict-aliasing

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

PORT = 12347

.PHONY: all clean

all: dir server

dir:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/server $(PORT)


server: $(OBJ)
	$(CC) -o $(BIN)/server $(OBJ)

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
