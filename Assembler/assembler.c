#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

/** Global Variables */
char *rTypeOpcodes = "add|nor";
char *iTypeOpcodes = "lw|sw|beq";
char *jTypeOpcodes = "jalr";
char *oTypeOpcodes = "halt|noop";

typedef struct LABEL
{
    char name[7];
    char value[MAXLINELENGTH];
    int address;
    struct LABEL *next;
} LABEL;

LABEL labels;
int labelCount;

/** Utils Functions */
int readAndParse(FILE *, char *, char *, char *, char *, char *);

void buildLabels(FILE *readFilePtr);
int getValueInLabels(char *);

/** Error Check Functions */
int isNumber(char *);
int isAlphabet(char *);

int isValidLabel(char *);
int isValidRegister(char *, int);

int isInOpcodes(char *, char *);

void abortWithError(char *string);

/** Formatting Functions */
int formatRTypeInst(char *opcode, char *reg0, char *reg1, char *destReg);
int formatITypeInst(char *opcode, char *reg0, char *reg1, char *offset, int pc);
int formatJTypeInst(char *reg0, char *reg1);
int formatOTypeInst(char *opcode);

int main(int argc, char *argv[])
{
    int line = 0;
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3)
    {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL)
    {

        printf("error in opening %s\n", inFileString);

        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL)
    {

        printf("error in opening %s\n", outFileString);

        exit(1);
    }

    /** Build Label Data for Assmebler */
    buildLabels(inFilePtr);

    // Assembler Main Logic
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        int mc = 0;

        if (isInOpcodes(opcode, rTypeOpcodes))
        {
            mc = formatRTypeInst(opcode, arg0, arg1, arg2);
        }
        else if (isInOpcodes(opcode, iTypeOpcodes))
        {
            mc = formatITypeInst(opcode, arg0, arg1, arg2, line);
        }
        else if (isInOpcodes(opcode, jTypeOpcodes))
        {
            mc = formatJTypeInst(arg0, arg1);
        }
        else if (isInOpcodes(opcode, oTypeOpcodes))
        {
            mc = formatOTypeInst(opcode);
        }
        else if (!strcmp(opcode, ".fill"))
        {
            if (isNumber(arg0))
            {
                mc = atoi(arg0);
            }
            else
            {
                mc = getValueInLabels(arg0);
            }
        }
        else
        {
            abortWithError("Do not support its opcode.");
        }

        fprintf(outFilePtr, "%d\n", mc);
        printf("(Address %d): %d\n", line, mc);

        line++;
    }

    fclose(inFilePtr);
    fclose(outFilePtr);
    exit(0);
}

/**
 * Read and parse a line of the assembly-language file. Fields are returned * in label, opcode, arg0, arg1, arg2 (these strings must have memory already * allocated to them).
 * Return values:
 * 0 if reached end of file 
 * 1 if all went well
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
    {
        /* reached end of file */
        return (0);
    }

    /* check for line too long (by looking for a \n) */
    if (strchr(line, '\n') == NULL)
    {
        /* line too long */
        printf("error: line too long\n");
        exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label))
    {
        /* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /**
     * Parse the rest of the line. Would be nice to have real regular * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
    return (1);
}

/**
 * Newly Defined Func]
 * 
 * Build Label Data for Assembler
 */
void buildLabels(FILE *readFilePtr)
{
    LABEL *current;
    int line = 0;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    labelCount = 0;
    while (readAndParse(readFilePtr, label, opcode, arg0, arg1, arg2))
    {
        // Early Continue;
        if (!strcmp(label, ""))
        {
            line++; // next line
            continue;
        }

        // Check If Label is valid
        if (!isValidLabel(label))
        {
            abortWithError("Label is not Valid.");
        }

        // Check If Label is duplicated
        for (current = &labels; current; current = current->next)
        {
            if (!strcmp(current->name, label))
            {
                abortWithError("Label is duplicated.");
            }
        }

        // Move Current Pointer to End of Labels
        for (current = &labels; current->next; current = current->next)
            ;

        current->next = malloc(sizeof(LABEL));
        current = current->next;

        strcpy(current->name, label);
        current->address = line++;

        if (!strcmp(opcode, ".fill"))
        {
            if (isNumber(arg0))
            {
                // long is 32bit, then overflow check do not fit. So, 64bit long long declare
                long long temp = atoll(arg0);
                // .fill Value Overflow Check
                if (temp > 2147483647 || temp < -2147483648)
                {
                    abortWithError(".fill value overflow.");
                }
            }

            strcpy(current->value, arg0);
        }
        else
        {
            current->value[0] = '\0';
        }

        labelCount++;
    }

    rewind(readFilePtr);
}

/**
 * Returns Label Value in Labels
 */
int getValueInLabels(char *name)
{
    LABEL *current;
    for (current = &labels; current; current = current->next)
    {
        if (!strcmp(current->name, name))
        {
            return current->address;
        }
    }

    abortWithError("undefined label");

    return 0;
}

/**
 * Returns Check If Number
 */
int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return ((sscanf(string, "%d", &i)) == 1);
}

/**
 * Newly Defined Func]
 * 
 * Returns Check If Alphabet
 */
int isAlphabet(char *string)
{
    return (int)((*string >= 'a' && *string <= 'z') || (*string >= 'A' && *string <= 'Z'));
}

/**
 * Newly Defined Func]
 * 
 * Returns Check If Label Rules
 * 
 * Rules
 * - Maximum String Length Not Exceed 6
 * - Left-most Character is not Number
 * - Use Only Number and Letter
 */
int isValidLabel(char *string)
{
    int i, strLength = 0;

    strLength = strlen(string);
    // Maximum String Length Check
    if (strLength > 6)
    {
        return 0;
    }

    // Left-most Character is not Number check
    if (isNumber(&string[0]))
    {
        return 0;
    }

    // Use Only Number and Letter Check
    for (i = 0; i < strLength; i++)
    {
        if (isAlphabet(&string[i]) || isNumber(&string[i]))
        {
            continue;
        }

        return 0;
    }

    // All Rules Passed
    return 1;
}

/**
 * Newly Defined Func]
 * 
 * Returns Check If Register Rules
 * 
 * Rules
 * - reg is Number
 * - reg Number Range is in 0 ~ 7 (Register Number from 0 to 7)
 */
int isValidRegister(char *reg, int isDestReg)
{
    int regValue = 0;

    if (!isNumber(reg))
    {
        return 0;
    }

    regValue = atoi(reg);
    if (regValue < 0 || regValue >= 8)
    {
        return 0;
    }

    if (isDestReg && regValue == 0)
    {
        return 0;
    }

    return 1;
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
 * Check If opcode is in opcodes Array
 */
int isInOpcodes(char *needle, char *opcodes)
{
    if (strstr(opcodes, needle) != NULL)
    {
        return 1;
    }

    return 0;
}

/**
 * Newly Defined Func]
 * 
 * Returns R-Type Format (ADD, NOR)
 * 
 * Format
 * bits 31-25: unused (should all be 0)
 * bits 24-22: opcode
 * bits 21-19: reg A
 * bits 18-16: reg B
 * bits 15-3: unused (should all be 0)
 * bits 2-0: destReg
 */
int formatRTypeInst(char *opcode, char *reg0, char *reg1, char *destReg)
{
    int mc = 0;

    // Valid Check All Reg
    if (!isValidRegister(reg0, 0) || !isValidRegister(reg1, 0) || !isValidRegister(destReg, 1))
    {
        abortWithError("Registers are not valid.");
    }

    // Opcode Formatting
    if (!strcmp(opcode, "add"))
    {
        mc = (0 << 22);
    }
    else if (!strcmp(opcode, "nor"))
    {
        mc = (1 << 22);
    }
    else
    {
        abortWithError("Wrong Opcode for R Type Inst Formatting");
    }

    // Address Formatting
    mc |= (atoi(reg0) << 19);
    mc |= (atoi(reg1) << 16);
    mc |= (atoi(destReg) << 0);

    return mc;
}

/**
 * Newly Defined Func]
 * 
 * Returns I-Type Format (with check offsetField logic) (LW, SW, BEQ)
 * 
 * Format
 * bits 31-25: unused (should all be 0)
 * bits 24-22: opcode
 * bits 21-19: reg A
 * bits 18-16: reg B
 * bits 15-0: offsetField (a 16-bit, 2's complement number with a range of -32768 to 32767)
 */
int formatITypeInst(char *opcode, char *reg0, char *reg1, char *offset, int pc)
{
    int mc = 0, address = 0;

    // Valid Check All Reg
    if (!isValidRegister(reg0, 0) || !isValidRegister(reg1, 0))
    {
        abortWithError("Registers are not valid.");
    }

    // Opcode Formatting
    if (!strcmp(opcode, "lw"))
    {
        mc |= (2 << 22);
    }
    else if (!strcmp(opcode, "sw"))
    {
        mc |= (3 << 22);
    }
    else if (!strcmp(opcode, "beq"))
    {
        mc |= (4 << 22);
    }
    else
    {
        abortWithError("Wrong opcode for I Type Inst Formatting");
    }

    // Address Formatting
    mc |= (atoi(reg0) << 19);
    mc |= (atoi(reg1) << 16);

    if (isNumber(offset))
    {
        address = atoi(offset);
        if (address > 32767 || address < -32768)
        {
            abortWithError("Offset out of boundary");
        }
    }
    else
    {
        address = getValueInLabels(offset);
    }

    if (!strcmp(opcode, "beq") && !isNumber(offset))
    {
        int dest = address;
        address = dest - pc - 1;
        if (address > 32767 || address < -32768)
        {
            abortWithError("Offset out of boundary");
        }
    }

    address &= 0xFFFF;
    mc |= address;

    return mc;
}

/**
 * Newly Defined Func]
 * 
 * Returns J-Type Format (JALR)
 * 
 * Format
 * bits 31-25: unused (should all be 0)
 * bits 24-22: opcode
 * bits 21-19: reg A
 * bits 18-16: reg B
 * bits 15-0: unused
 */
int formatJTypeInst(char *reg0, char *reg1)
{
    int mc = 0;

    // Valid Check All Reg
    if (!isValidRegister(reg0, 0) || !isValidRegister(reg1, 0))
    {
        abortWithError("Registers are not valid.");
    }

    // Opcode Formatting
    mc = (5 << 22);

    // Register Formatting
    mc |= (atoi(reg0) << 19);
    mc |= (atoi(reg1) << 16);

    return mc;
}

/**
 * Newly Defined Func]
 * 
 * Returns O-Type Format (HALT, NOOP)
 * 
 * Format
 * bits 31-25: unused (should all be 0)
 * bits 24-22: opcode
 * bits 21-0: unused
 */
int formatOTypeInst(char *opcode)
{
    int mc = 0;

    // Opcode Formatting
    if (!strcmp(opcode, "halt"))
    {
        mc = (6 << 22);
    }
    else if (!strcmp(opcode, "noop"))
    {
        mc = (7 << 22);
    }
    else
    {
        abortWithError("Wrong Opcode for O Type Inst Formatting");
    }

    return mc;
}