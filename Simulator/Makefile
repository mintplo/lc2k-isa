CC = gcc
OBJS = simulator.o
TARGET = simulator
 
.SUFFIXES : .c .o
 
all : $(TARGET)
 
$(TARGET): $(OBJS)
	   $(CC) -o $@ $(OBJS)
 
clean :
	rm -f $(OBJS) $(TARGET)