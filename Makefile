CC     = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC    = src/main.c src/parser.c
OBJ    = $(SRC:.c=.o)
TARGET = schedsim

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) src/*.o

test: all
	./schedsim --algorithm=FCFS --input=tests/workload1.txt
	@echo ""
	./schedsim --algorithm=RR --quantum=50 --processes="A:0:240,B:10:180,C:20:150"

.PHONY: all clean test
