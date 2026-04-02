CC       = gcc
CFLAGS   = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS  = -lSOIL -lglut -lGLEW -lGL -lm

SRC_DIR   = src
BUILD_DIR = build
OBJ_DIR   = $(BUILD_DIR)/obj

SOURCES  = $(wildcard $(SRC_DIR)/*.c)
OBJECTS  = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET   = $(BUILD_DIR)/magic-journey

# Test config (no OpenGL dependency)
TEST_SRCS    = $(wildcard tests/test_*.c)
TEST_BINS    = $(TEST_SRCS:tests/%.c=$(BUILD_DIR)/%)
TESTABLE_SRC = $(SRC_DIR)/game.c $(SRC_DIR)/physics.c $(SRC_DIR)/input.c
UNITY_SRC    = vendor/unity/unity.c

# Platform detection
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    BREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)
    CFLAGS  += -I$(BREW_PREFIX)/include -DGL_SILENCE_DEPRECATION
    SOIL_LIB = $(BREW_PREFIX)/lib/libSOIL.a
    LDFLAGS  = -L$(BREW_PREFIX)/lib $(SOIL_LIB) -framework GLUT -framework OpenGL -framework CoreFoundation -lm
endif
ifeq ($(UNAME), Linux)
    LDFLAGS = -lSOIL -lglut -lGLEW -lGL -lm
endif
ifeq ($(OS), Windows_NT)
    LDFLAGS = -D FREEGLUT_STATIC -lSOIL -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,windows
endif

.PHONY: all run test lint clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	cd $(CURDIR) && ./$(TARGET)

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do echo "\n--- Running $$t ---"; ./$$t || exit 1; done
	@echo "\nAll tests passed."

$(BUILD_DIR)/test_%: tests/test_%.c $(TESTABLE_SRC) $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -Ivendor/unity $^ -o $@ -lm

lint:
	cppcheck --enable=all --suppress=missingIncludeSystem -Iinclude $(SRC_DIR)/ tests/

clean:
	rm -rf $(BUILD_DIR)
