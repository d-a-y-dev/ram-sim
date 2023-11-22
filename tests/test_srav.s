
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
test_srav:
        addi $t3,  $zero, -13         # t3 -> -13
        addi $t0,  $zero, -100        # t0 -> -100
        addi $t1,  $zero, 35          # t1 -> 35
        srav $t2,  $t0, $t1           # t2 -> (-100) >> 3
        bne $t2,  $t3, inf            # Jump to exit if $t2 = -13
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
