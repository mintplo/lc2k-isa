        lw      0   3   mplier
        lw      0   4   maskin
        lw      0   5   maxbit
        lw      0   6   mcand
L1      nor     3   4   2
        beq     0   2   calc
        beq     0   0   next
calc    add     1   6   1
next    lw      0   2   neg1
        add     2   5   5
        beq     0   5   done
        add     4   4   4
        lw      0   2   pos1
        add     4   2   4
        add     6   6   6
        beq     0   0   L1
done    noop
        halt
mcand   .fill   32766
mplier  .fill   10383
maskin .fill   -2
pos1    .fill   1
neg1    .fill   -1
maxbit  .fill   15
