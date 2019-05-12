        lw      0   1   input           load reg1 with input(3) (symbolic address)
        lw      0   2   stack           load reg1 with stack(100) (symbolic address)
        lw      0   3   neg1            load reg3 with neg1(-1) (symbolic address)
        lw      0   4   pos1            load reg4 with pos1(1) (symbolic address)
        lw      0   5   arAddr          load reg5 with address of addR(recursive)
        jalr    5   6                   stores address of sw to reg6 and go to func
        sw      0   7   none            save reg3 to position none
        noop
        halt                            return value from recursive call will be saved at reg7 & mem50
addR    add     2   3   2               make room for the stack.
        add     2   3   2               make room for the stack.
        sw      2   6   1               store reg6(return address) into 4($reg2)
        sw      2   1   0               store reg1(input) into 0($reg2)
        beq     0   1   L1              if input reaches 0 go to L1
        add     1   3   1               decrement input
        lw      0   5   arAddr          load reg5 with address of addR(recursive)
        jalr    5   6                   recursive call
        lw      2   6   1               load reg6(return address) from 4($reg2)
        lw      2   1   0               load reg1(input) from 0($reg2)
        add     2   4   2               pop item from stack
        add     2   4   2               pop item from stack
        add     1   7   7               add input to return value(reg7)
        jalr    6   5                   return from function
L1      add     2   4   2               pop item from stack
        add     2   4   2               pop item from stack
        add     0   0   7               set reg7 to 0
        jalr    6   5                   return from function
input  .fill    3
neg1   .fill    -1
pos1   .fill    1
stack  .fill    100                     address of stack
none   .fill    0
arAddr .fill    addR
