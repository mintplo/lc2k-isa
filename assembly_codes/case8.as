        lw      0       1       input   load reg1 with 20 (symbolic address)
        add     1       1       2       save reg2 with input * 2
        add     2       2       3       save reg3 with input * 4
        add     3       3       3       save reg3 with input * 8
        add     2       3       1       save reg1 with input * (8 + 2)
        noop
        halt
input   .fill   20
pos1    .fill   1
mempos  .fill   100
