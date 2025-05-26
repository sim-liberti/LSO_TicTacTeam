# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET = build/server.exe
    RM = del /Q
    LDFLAGS = -lws2_32
    FOLDERS = build\lib build\utils
else
    TARGET = build/server.out
    RM = rm -f
    LDFLAGS =
    FOLDERS = build/lib build/utils
endif

CC = gcc
CFLAGS = -Wall -g -Iinclude -Ilib
SRCS = $(wildcard *.c) $(wildcard lib/*.c) $(wildcard utils/*.c)
OBJS = $(patsubst %.c, build/%.o, $(SRCS))

all: build $(TARGET)

build:
	mkdir -p $(FOLDERS)

$(TARGET): $(OBJS)
	$(CC) -static $(OBJS) -o $@ $(LDFLAGS)

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) build/*.o build/*/*.o $(TARGET)
