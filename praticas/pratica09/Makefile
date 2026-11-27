CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
TARGET = main
SRCS = grafo.c cobertura.c io.c main.c
LIB_SRCS = grafo.c cobertura.c
TEST_BIN = test_cobertura
TEST_SRC = tests/test_cobertura.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lm

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(LIB_SRCS) $(TEST_SRC)
	$(CC) $(CFLAGS) -I. -o $(TEST_BIN) $(LIB_SRCS) $(TEST_SRC) -lm

clean:
	rm -f $(TARGET) $(TARGET).exe $(TEST_BIN) $(TEST_BIN).exe *.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all test clean run
