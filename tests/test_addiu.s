
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
test_addiu_pos:
                                      # test ADDIU positive
        addiu $t0, $t0, 0x1a
        ori $t1, $t1, 0x1a
        bne $t0, $t1, inf
        jal reset
test_addiu_neg:
                                      # test ADDIU negative
        addiu $t0, $t0, -1
        lui $t1, 0xFFFF
        ori $t1, $t1, 0xFFFF          #0xFFFF0000
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
