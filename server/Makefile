# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET = build/server.exe
    RM = del /Q
    LDFLAGS = -lws2_32
else
    TARGET = build/server.out
    RM = rm -f
    LDFLAGS =
endif

CC = gcc
CFLAGS = -Wall -g -Iinclude -Ilib
SRCS = $(wildcard *.c) $(wildcard lib/*.c) $(wildcard utils/*.c)
OBJS = $(patsubst %.c, build/%.o, $(SRCS))

all: build $(TARGET)

build:
	mkdir -p build/lib build/utils

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) build/*.o build/*/*.o $(TARGET)
