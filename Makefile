CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lcurl

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = tateti

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(EXEC)
	rm -f ./informes/*


run: $(EXEC)
	./$(EXEC)