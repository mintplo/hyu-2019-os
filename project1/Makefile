CC = gcc
OBJS = semaphore.o
TARGET = semaphore

.SUFFIXES : .c .o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -lpthread

clean :
	rm -f $(OBJS) $(TARGET)
