## LC-2K ISA

2019 HYU Computer Architecture PROJECT#1

## Implementation

1. Assembler
2. Simulator
3. Multiplication Assembly Code

## Develop Environment

- GCC
  - Apple LLVM version 10.0.1 (clang-1001.0.46.4)
  - Target x86_64-apple-darwin18.5.0
  - Debug with LLDB
- Visual Studio Code

## Project Specifications

### Assembler

- Assembler should catch the following errors in the assembly-language program: use of undefined labels, duplicate labels, offsetFields that don't fit in 16 bits, and unrecognized opcodes.
- Assembler should exit(1) if it detects an error and exit(0) if it finishes without detecting any errors.
- Assembler should NOT catch simulation-time errors
- The bounds of the numeric value for .fill instructions are -2^31 to +2^31-1 (-2147483648 to 2147483647).
- Register 0 will always contain 0
- Since offsetField is a 2's complement number, it can only store numbers ranging from -32768 to 32767

### Simulator

- The LC-2K is an 8-register, 32-bit computer.
- All addresses are word-addresses.
- The LC-2K has 65536 words of memory

### Multiplication

- Input the numbers by reading memory locations called "mcand" and "mplier"
- You may assume that the two input numbers are at most 15 bits and are positive.
- Remember that shifting left by one bit is the same as adding the number to itself.
- Submit a version of the program that computes (32766 \* 10383)
- it must be at most 50 lines long and execute at most 1000 instructions for any valid input

## Build & Run

Build Assembler with Makefile

```bash
cd Assembler
make
./assembler testcases/case1.as testcases/case1.mc
```

Build Assembler with GCC

```bash
cd Assembler
gcc assembler.c -o assembler
./assembler testcases/case1.as testcases/case1.mc
```

Build Simulator with Makefile

```bash
cd Simulator
make
./simulator testcases/case1.mc > case1.txt
```

Build Simulator with GCC

```bash
cd Simulator
gcc simulator.c -o simulator
./simualtor testcases/case1.mc > case1.txt
```
