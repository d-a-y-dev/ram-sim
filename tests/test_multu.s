
# BLEZ BGTZ
# LB LH LW LBU
# LHU SB SH SW BLTZ BGEZ
# BLTZAL BGEZAL SRA 

# This test assumes that all jump and branching instruction works.

.text

main:   
        j start_test
reset:
        addi $t0, $zero, 0
        addi $t1, $zero, 0
        addi $t2, $zero, 0
        addi $t3, $zero, 0
        addi $t4, $zero, 0
        addi $t6, $t6, 1
        mthi $zero
        mtlo $zero
        jr $ra
start_test:
test_multu_pos:
        addi $t0, $t0, 333        
        addi $t1, $t1, -111
        multu $t1, $t0
        mfhi $t2
        mflo $t3
        ori $t1, $zero, 0x14c         # testing hi
        bne $t2, $t1, inf
        lui $t4, 0xffff               # testing lo
        ori $t4, $t4, 0x6f9d
        bne $t3, $t4, inf
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
