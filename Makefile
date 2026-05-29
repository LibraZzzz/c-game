CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic -Iinclude
BUILD_DIR := build
TARGET := $(BUILD_DIR)/minesweeper
ifeq ($(OS),Windows_NT)
TARGET := $(BUILD_DIR)/minesweeper.exe
endif

SOURCES := src/main.c src/board.c src/game.c
OBJECTS := $(SOURCES:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.c include/board.h include/game.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

run: all
	$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif
