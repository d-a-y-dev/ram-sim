
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
 test_subu:
                                      # test subu (pos result)
         ori $t0, $t0, 0xFF
         ori $t1, $t1, 0x4
         subu $t3, $t0, $t1
         ori $t4, $t4, 0xFB
         bne $t3, $t4, inf
         jal reset
test_subu_neg:
                                      # test subu (neg result)
        ori $t0, $t0, 0x0
        ori $t1, $t1, 0x4
        subu $t3, $t0, $t1
        lui $t4, 0xFFFF
        ori $t4, $t4, 0xFFFC
        bne $t3, $t4, inf
        jal reset
test_subu_neg_1:
                                      # test subu (neg result)
        ori $t0, $t0, 0x4
        ori $t1, $t1, 0x0
        subu $t3, $t0, $t1
        addi $t4, $zero, 4
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
