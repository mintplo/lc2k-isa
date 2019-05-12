## LC-2K ISA

2019 HYU Computer Architecture PROJECT#1

### Implementing

1. Assembler
2. Simulator
3. Multiplication Assembly Code

### Develop Environment

- GCC
  - Apple LLVM version 10.0.1 (clang-1001.0.46.4)
  - Target x86_64-apple-darwin18.5.0
  - Debug with LLDB
- Visual Studio Code

### Project Specifications

TBD

### Build & Run

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
