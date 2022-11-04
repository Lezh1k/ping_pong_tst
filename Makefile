#CC=gcc
CC=clang
AS=as
LD=ld
LINK=$(CC)

BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=bin

LIBS := -lm -lpthread
WARN_LEVEL = -Wall -Wextra -pedantic

PRG = pp_tst
INCLUDES = -Iinc
CFLAGS := $(INCLUDES) $(DEFS) $(WARN_LEVEL) -pipe -std=gnu18
debug: CFLAGS += -O0 -g3
debug: all
release: CFLAGS += -O2
release: all

LDFLAGS = $(LIBS) #-ffunction-sections -Wl,--gc-sections

SRC_C := $(wildcard *.c) $(wildcard src/*.c)
OBJECTS := $(SRC_C:%.c=$(OBJ_DIR)/%.o)

all: directories $(PRG)
$(PRG): $(BIN_DIR)/$(PRG)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN_DIR)/$(PRG): $(OBJECTS)
	@mkdir -p $(@D)
	$(LINK) -o $(BIN_DIR)/$(PRG).elf $^ $(LDFLAGS) $(LIBS)

.PHONY: directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(BIN_DIR)/*

.PHONY: mrproper
mrproper:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
