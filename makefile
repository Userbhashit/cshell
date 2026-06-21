CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c23 -Iinclude

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

TARGET = cshell

SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(INC_DIR)/*.h)  # <-- Added: Automatically tracks all header files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
