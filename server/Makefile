# Nome dell'eseguibile finale
TARGET = main.exe

CC = gcc
CFLAGS = -Wall
LDFLAGS = -lws2_32

SRCS = $(wildcard *.c) utils/cJSON.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	del /Q *.o *.exe
