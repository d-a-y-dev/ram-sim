
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
test_slt0:
        addi $t0, $zero, -1           # t0 = -1
        addi $t2, $zero, -1           # t2 = -1
        slt $t1, $t0, $t2             # t1 = 0 
        bne $t1, $zero, inf   
        jal reset
test_slt1:
        addi $t0, $zero, -1           # t0 = -1
        addi $t2, $zero, 1            # t2 = 1
        slt $t1, $t0, $t2             # t1 = 1 
        beq $t1, $zero, inf   
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
