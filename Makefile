CC= gcc -fPIC
CFLAGS= -Wall -Wpedantic -Wextra -O2
LIBS=-lm
SRCDIR= ./src
OBJDIR= ./obj
BINDIR= ./bin

SRC=graph.c hash_table.c priority_queue.c list.c main.c

OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(BINDIR)/main

$(BINDIR)/main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)
.PHONY=clean
clean:
	rm $(BINDIR)/* $(OBJDIR)/*.o
