CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lcurl

SRC = $(wildcard *.c) $(wildcard api/*.c) $(wildcard informes-funciones/*.c) $(wildcard primitivas/*.c)
OBJ = $(patsubst %.c,archivos-temporales/%.o,$(notdir $(SRC)))
EXEC = tateti

all: archivos-temporales $(EXEC)

archivos-temporales:
	mkdir -p archivos-temporales

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

archivos-temporales/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

archivos-temporales/%.o: api/%.c
	$(CC) $(CFLAGS) -c $< -o $@

archivos-temporales/%.o: informes-funciones/%.c
	$(CC) $(CFLAGS) -c $< -o $@

archivos-temporales/%.o: primitivas/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f archivos-temporales/*.o $(EXEC)
	rm -rf archivos-temporales
	rm -f ./informes/*

run: all
	./$(EXEC)