
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
test_divu_pos:
        ori $t0, $t0, 421
        ori $t1, $t1, 107
        divu $t0, $t1
        mfhi $t2
        mflo $t3
        addi $t0, $zero, 3
        ori $t1, $zero, 0x64
        bne $t3, $t0, inf
        bne $t2, $t1, inf
        jal reset
test_divu_neg:
        ori $t0, $t0, 421
        addi $t1, $t1, -107
        divu $t0, $t1
        mfhi $t2
        mflo $t3
        addi $t0, $zero, 3
        bne $t3, $zero, inf
        ori $t1, $zero, 0x1a5
        bne $t2, $t1, inf
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
