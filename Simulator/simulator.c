/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8       /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct
{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

/** Util + Error Check Functions */
void printState(stateType *);
void abortWithError(char *string);

int isValidRegister(int reg);

int convertNum(int num);

/** Simulator Functions */
void parseInstFromMem(stateType *statePtr, int *opcode, int *arg0, int *arg1, int *arg2);

void executeRTypeInst(stateType *statePtr, int opcode, int arg0, int arg1, int dest);
void executeITypeInst(stateType *statePtr, int opcode, int arg0, int arg1, int offset);
void executeJTypeInst(stateType *statePtr, int opcode, int arg0, int arg1);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    int executionCount = 0;

    if (argc != 2)
    {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL)
    {

        printf("error: can't open file %s", argv[1]);

        perror("fopen");

        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++)
    {

        if (sscanf(line, "%d", state.mem + state.numMemory) != 1)
        {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    // Print initial state
    printState(&state);
    while (1)
    {
        int isHalt = 0;
        int opcode, arg0, arg1, arg2;
        parseInstFromMem(&state, &opcode, &arg0, &arg1, &arg2);

        state.pc++;
        executionCount++;

        // PC Range Check
        if (state.pc < 0 || state.pc >= NUMMEMORY)
        {
            abortWithError("PC out of memory");
        }

        /**
         * OPCODES
         * 
         * 0 - add
         * 1 - nor
         * 2 - lw
         * 3 - sw
         * 4 - beq
         * 5 - jalr
         * 6 - halt
         * 7 - noop
         */
        switch (opcode)
        {
        case 0: // [R-TYPE] add
        case 1: // [R-TYPE] nor
            executeRTypeInst(&state, opcode, arg0, arg1, arg2);
            break;
        case 2: // [I-TYPE] lw
        case 3: // [I-TYPE] sw
        case 4: // [I-TYPE] beq
            executeITypeInst(&state, opcode, arg0, arg1, arg2);
            break;
        case 5: // [J-TYPE] jalr
            executeJTypeInst(&state, opcode, arg0, arg1);
            break;
        // O-Type Insts have no exeucte functions (only in switch case)
        case 6: // [O-TYPE] halt
            isHalt = 1;
            break;
        case 7: // [O-TYPE] noop
            // NOOP Opcode is do nothing
            break;
        default:
            abortWithError("Do not support its opcode.");
            break;
        }

        // [O-TYPE] halt execution in while loop
        if (isHalt)
        {
            break;
        }

        // Print state
        printState(&state);
    }

    printf("machine halted\n");
    printf("total of %d instructions executed\n", executionCount);
    printf("final state of machine:\n");

    printState(&state);

    fclose(filePtr);
    exit(0);
}

/**
 * Print State in State Type
 */
void printState(stateType *statePtr)
{

    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {

        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {

        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

/**
 * Newly Defined Func]
 * 
 * abortWithError with Error Print
 */
void abortWithError(char *string)
{
    fprintf(stderr, "error: %s\n", string);
    exit(1);
}

/**
 * Newly Defined Func]
 * 
 * Register Valid Check
 */
int isValidRegister(int reg)
{
    return (int)(reg >= 0 && reg < NUMREGS);
}

/**
 * Convert 16bit NUM to 32bit NUM
 */
int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1 << 15))
    {
        num -= (1 << 16);
    }

    return (num);
}

/**
 * Newly Defined Func]
 * 
 * Parse Instruction From Memory with State Type
 */
void parseInstFromMem(stateType *statePtr, int *opcode, int *arg0, int *arg1, int *arg2)
{
    int memValue = statePtr->mem[statePtr->pc];

    // Extract 25 ~ 22 bit and save opcode
    *opcode = (memValue >> 22) & 0b111;
    // Extract 21-19 bit and save binary to arg0
    *arg0 = (memValue >> 19) & 0b111;
    // Extract 18-16 bit and save binary to arg1
    *arg1 = (memValue >> 16) & 0b111;
    // Extract 15-0 bit and save binary to arg2
    *arg2 = (memValue & 0xFFFF);
}

/**
 * Newly Defined Func]
 * 
 * ADD: (opcode 0)
 * Add contents of arg0 with contents of arg1, store results in destReg.
 * 
 * NOR: (opcode 1)
 * Nor contents of arg0 with contents of arg1, store results in destReg.
 * This is a bitwise nor; each bit is treated independently.
 * 
 * Format
 * bits 24-22: opcode
 * bits 21-19: reg A
 * bits 18-16: reg B
 * bits 2-0: destReg
 */
void executeRTypeInst(stateType *statePtr, int opcode, int arg0, int arg1, int dest)
{
    // Check REG
    if (!isValidRegister(arg0) || !isValidRegister(arg1) || !isValidRegister(dest))
    {
        abortWithError("Register is not valid.");
    }

    switch (opcode)
    {
    case 0: // add
        statePtr->reg[dest] = statePtr->reg[arg0] + statePtr->reg[arg1];
        break;
    case 1: // nor
        statePtr->reg[dest] = ~(statePtr->reg[arg0] | statePtr->reg[arg1]);
        break;
    default:
        abortWithError("Do not support its opcode");
        break;
    }
}

/**
 * Newly Defined Func]
 * 
 * LW: (opcode 2)
 * Load arg1 from memory.
 * Memory address is formed by adding offsetField with the contents of arg0.
 * 
 * SW: (opcode 3)
 * Store arg1 into memory.
 * Memory address is formed by adding offsetField with the contents of arg0.
 * 
 * BEQ: (opcode 4)
 * If the contents of arg0 and arg1 are the same,
 * then branch to the address PC+1+offsetField,
 * where PC is the address of this beq instruction
 * 
 * Format
 * bits 24-22: opcode
 * bits 21-19: reg A
 * bits 18-16: reg B
 * bits 15-0: offsetField (a 16-bit, 2's complement number with a range of -32768 to 32767)
 */
void executeITypeInst(stateType *statePtr, int opcode, int arg0, int arg1, int offset)
{
    offset = convertNum(offset);

    if (!isValidRegister(arg0) || !isValidRegister(arg1))
    {
        abortWithError("Register is not valid.");
    }

    if (offset > 32767 || offset < -32768)
    {
        abortWithError("Offset out of range");
    }

    switch (opcode)
    {
    case 2:
        statePtr->reg[arg1] = statePtr->mem[statePtr->reg[arg0] + offset];
        break;
    case 3:
        statePtr->mem[statePtr->reg[arg0] + offset] = statePtr->reg[arg1];
        break;
    case 4:
        if (statePtr->reg[arg0] == statePtr->reg[arg1])
        {
            statePtr->pc += offset;
        }
        break;
    default:
        abortWithError("Do not support its opcode");
        break;
    }
}

/**
 * Newly Defined Func]
 * 
 * JALR: (opcode 5)
 * First store PC+1 into arg1, where PC is the address of this jalr instruction.
 * Then branch to the address contained in arg0.
 * ==
 * Note that this implies if arg0 and arg1 refer to the same register, the net effect will be jumping to PC+1.
 * 
 * 
 * Format
 * bits 24-22: opcode
 * bits 21-19: reg A
 * bits 18-16: reg B
 * bits 15-0: unused
 */
void executeJTypeInst(stateType *statePtr, int opcode, int arg0, int arg1)
{
    if (!isValidRegister(arg0) || !isValidRegister(arg1))
    {
        abortWithError("Register is not valid.");
    }

    switch (opcode)
    {
    case 5:
        statePtr->reg[arg1] = statePtr->pc;
        statePtr->pc = statePtr->reg[arg0];
        break;
    default:
        abortWithError("Do not support its opcode");
        break;
    }
}
