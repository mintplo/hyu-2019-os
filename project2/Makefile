CC = gcc
OBJS = scehduler.o
TARGET = scehduler
 
.SUFFIXES : .c .o
 
all : $(TARGET)
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) -g
 
clean :
	rm -f $(OBJS) $(TARGET)