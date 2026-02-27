CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lm

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = bin/compressor

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

# Nova regra para rodar os testes
test: all
	$(CC) $(CFLAGS) src/test_matrix.c $(OBJ_DIR)/matrix.o -o bin/test_matrix $(LDFLAGS)
	./bin/test_matrix

testar: testar.c src/matrix.c
	$(CC) $(CFLAGS) testar.c src/matrix.c -o testar $(LDFLAGS)