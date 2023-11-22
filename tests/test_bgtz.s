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
test_bgtz_not_taken:
        bgtz $t0, inf
        jal reset
test_bgtz_taken_1:
	addi $t0, $zero, 5
	bgtz $t0 bgtz_take_5
	j inf
bgtz_take_5:
	jal reset
test_bgtz_not_taken_2:
	addi $t0, $zero, -1
        bgtz $t0, inf
        jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
