        lw      0       1       pos1    load reg1 with 1
        lw      0       2       neg1    load reg2 with -1
        lw      0       3       cntAdr  load reg3 with address of cnt function
        jalr    3       7
        noop
done    halt                            end of program
cnt     lw      0       4       addrA   load start address of A array
        add     0       0       3
L1      lw      4       5       0       load reg5 with A[reg4]
        beq     0       5       start
        add     1       3       3
        add     1       4       4
        beq     0       0       L1      count elements in A array
start   add     0       3       1       store counted elements into reg1
        jalr    7       3
pos1    .fill   1
neg1    .fill   -1
cntAdr  .fill   cnt
addrA   .fill   A0
sim1    .fill   11111111                to find A array easily on simulator
A0    .fill   4
A1    .fill   5
A2    .fill   3
A3    .fill   1
A4    .fill   2
A5    .fill   0                       end of array(not considered as part of array)
sim2    .fill   11111111                to find A array easily on simulator
