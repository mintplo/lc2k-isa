        lw 0 2 mplier
        lw 0 3 mcand
        lw 0 4 maskb
        lw 0 5 maxbit
        noop            === logic divider ===
        beq 0 0 loop
loop    nor 2 4 7       extract one bit doing bit masking bit 0 equal 1 (beacuse nor)
        beq 0 7 calc
        beq 0 0 proc    Do not executes calc and then skip calc, do proc
calc    add 1 3 1       add mcand (left shifted) for binary multiplication
proc    lw 0 7 sub1     load negative 1 value to reg7 (temporary reg)
        add 5 7 5       max bit sub 1 whenever bit operation loop executes
        beq 0 5 done    machine halt if max bit equal 0
        add 3 3 3       left bit shift by adding a register to itself
        add 4 4 4       left bit shift by adding a register to itself (maskbit)
        lw 0 7 add1     load positive 1 value to reg7 (temporary reg)
        add 4 7 4       adding 1 to left shifted maskbit
        beq 0 0 loop    continue loop
done    noop            === logic divider ===
        halt
mcand   .fill   32766
mplier  .fill   10383
maskb   .fill   -2      for mask bit ~ ~ 1111 1110 (using nor) and shift left + 1
add1    .fill   1
sub1    .fill   -1
maxbit  .fill   15      for only 15bit calculation
