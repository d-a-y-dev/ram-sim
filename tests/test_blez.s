
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
test_blez_not_taken:
        addi $t0, $zero, 100
        blez $t0, inf
        jal reset
test_blez_taken_1:
	blez $t0 blez_take_zero
	j inf
blez_take_zero:
	jal reset
test_blez_taken_2:
	addi $t0, $zero, -25
	blez $t0 blez_take_neg
	j inf 				# jump over this
blez_take_neg:
	jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
