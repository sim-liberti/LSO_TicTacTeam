# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET = server.exe
    RM = del /Q
    LDFLAGS = -lws2_32
else
    TARGET = server.out
    RM = rm -f
    LDFLAGS =
endif

CC = gcc
CFLAGS = -Wall -g -Iinclude -Ilib
SRCS = $(wildcard *.c) $(wildcard lib/*.c) $(wildcard utils/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(TARGET)
