CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
TARGET = main
SRCS = grafo.c hamiltoniano.c tps.c io.c main.c
LIB_SRCS = grafo.c hamiltoniano.c tps.c
TEST_BIN = test_pratica10
TEST_SRC = tests/test_pratica10.c

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
