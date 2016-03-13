CC = gcc
CFLAGS = -fPIC -pedantic -Wall -g
LD = ld
LIBS = -L/usr/local/lib
INCLUDES = -I./

SRC = $(wildcard *.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))
OBJD = $(addprefix obj/,$(notdir $(SRC:.c=.debug.o)))

NAME = bin/interpreter

#
# Dependencies
includes = $(wildcard *.h)

all: build

clean:
	rm -rf interpreter obj/*.o

obj/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< $(INCLUDES)
obj/%.debug.o: %.c
	$(CC) -c $(CFLAGS) -DDEBUG -o $@ $< $(INCLUDES)

build: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

debug: $(OBJD)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJD) $(LIBS)

.PHONY: clean
