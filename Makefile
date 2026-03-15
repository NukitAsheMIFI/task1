CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2
TARGET = main
SRCS = main.c task1.c 
OBJS = $(SRCS:.c=.o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
%.o: %.c task1.h
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
