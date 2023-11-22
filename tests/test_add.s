
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
test_add_pos:
                                      # test add positive
        ori $t1, $t1, 0x1a
        add $t0, $t0, $t1
        bne $t0, $t1, inf
        jal reset
test_add_neg:
                                      # test add negative
        addi $t2, $t2, -1
        add $t0, $t0, $t2
        lui $t1, 0xFFFF
        ori $t1, $t1, 0xFFFF
        bne $t0, $t1, inf
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
