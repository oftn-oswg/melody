CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -O3 -march=native -mtune=native -pipe `pkg-config --cflags smf`
LDFLAGS = `pkg-config --libs smf`

EXE = melody
SRC = melody.c music_parser.c music_generate.c
OBJ = ${SRC:.c=.o}

.PHONY: all clean

all: $(EXE)

.c.o:
	$(CC) -c $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	strip --strip-all $@

clean:
	-@rm -rf $(OBJ) $(EXE)
