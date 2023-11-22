
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
test_jalr:
        addi $t3, $zero, 5
        jal jump_label_1
        jal jump_label_2 
        j jump_exit_label 
jump_label_1:
        add $t0, $zero, $ra 
        beq $t2, $t3, jump_okay
        jr $ra
jump_label_2:
        jr $ra 
jump_exit_label:
        addi $t1, $t1, 4
        sub $ra, $ra, $t1 
        bne $ra, $t0, inf 
        addi $t2, $zero, 5
        addi $t0, $t0, 8
        jalr $t0 
jump_okay:
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
