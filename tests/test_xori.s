
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
test_xori_00:
        addi $t0, $zero, 0
        xori $t2, $t0, 0
        bne $t2, $zero, inf
        jal reset
test_xori_01:
        addi $t0, $zero, 0
        xori $t2, $t0, 1
        beq $t2, $zero, inf
        jal reset
test_xori_11:
        addi $t0, $zero, 1
        xori $t2, $t0, 1
        bne $t2, $zero, inf
        jal reset
test_xori_10:
        addi $t0, $zero, 1
        xori $t2, $t0, 0
        beq $t2, $zero, inf
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
