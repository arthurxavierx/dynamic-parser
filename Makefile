CC = gcc
CFLAGS = -fPIC -pedantic -Wall -g
LD = ld
LIBS = -L/usr/local/lib
INCLUDES = -I./

SRC = $(wildcard *.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))

NAME = interpreter

#
# Dependencies
includes = $(wildcard *.h)

all: build

clean:
	rm -rf interpreter obj/*.o

obj/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDES)

build: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

.PHONY: clean
