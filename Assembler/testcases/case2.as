        lw      0   1   input           load reg1 with input(3) (symbolic address)
        lw      0   2   cnt             load reg2 with cnt(10) (symbolic address)
        add     0   0   3               load reg3 with 0 (numeric address)
        lw      0   4   neg1            load reg4 with -1 (symbolic address)
        lw      0   5   fAdr            load reg5 with address of func
        jalr    5   6                   stores address of sw to reg6 and go to func
        sw      0   3   100             save reg3 to offset 100
        halt
func    add     3   1   3               add input to reg3 saves it to reg3
        add     2   4   2               decrement reg2
        beq     0   2   done            if cnt reaches 0 go to halt
        beq     0   0   func
done    jalr    6   7
fAdr   .fill    func
cnt    .fill    10
input  .fill    3
neg1   .fill    -1
