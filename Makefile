CC = gcc
ASSM_OBJS = assembler.o
SIM_OBJS = simulator.o
ASSM_TARGET = assembler
SIM_TARGET = simulator

.SUFFIXES : .c .o

all: assembler simulator

assembler: $(ASSM_OBJS)
		$(CC) -o $@ $(ASSM_OBJS)
simulator: $(SIM_OBJS)
		$(CC) -o $@ $(SIM_OBJS)

clean :
	rm -f $(ASSM_OBJS) $(ASSM_TARGET) $(SIM_OBJS) $(SIM_TARGET)
