        lw      0       1       one     load reg1 with 1 (symbolic address)
        lw      0       2       two     load reg2 with 2 (numeric address)
        add     0       1       3       store reg1 to reg3
        add     0       2       1       swap reg1 <-> reg2
        add     0       3       2       swap reg1 <-> reg2
        noop
done    halt                            end of program
one    .fill   1
two    .fill   2
