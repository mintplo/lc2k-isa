        lw      0       1       input   load reg1 with 20 (symbolic address)
        lw      0       2       pos1    load reg2 with 1  (symbolic address)
        lw      2       5       pos1    load reg5 with mempos
        nor     1       0       3       perform nor operation on reg0 & reg1 saves to reg3
        nor     1       1       4       perform nor operation on reg1 & reg1 saves to reg4
        add     2       3       3       = 2's complement of input
        add     2       4       4       = 2's complement of input
        sw      5       3       0       store data
        sw      5       4       1       store data
        noop
        halt
input   .fill   20
pos1    .fill   1
mempos  .fill   none1
none1   .fill   0
none2   .fill   0
