        lw      0       1       input           r1 = memory[input]
        lw      0       4       subAdr          prepare to call sub4n. r4=sub4n
        jalr    4       7                       call sub4n; r7=return address; r3=answer
        halt
sub4n   lw      0       6       pos1            r6 = 1
        sw      5       7       stack           save return address on stack
        add     5       6       5               increment stack pointer
        sw      5       1       stack           save input on stack
        add     5       6       5               increment stack pointer
        add     1       1       1               compute 2*input
        add     1       1       3               compute 4*input into return value
        lw      0       6       neg1            r6 = -1
        add     5       6       5               decrement stack pointer
        lw      5       1       stack           recover original input
        add     5       6       5               decrement stack pointer
        lw      5       7       stack           recover original return address
        jalr    7       4                       return.         r4 is not restored.
pos1    .fill   1
neg1    .fill   -1
subAdr  .fill   sub4n                           contains the address of sub4n
input   .fill   10                              input = 10
stack   .fill   0                               beginning of stack (value is irrelevant)
stack1  .fill   0                               lets us see stack+1 in simulation
