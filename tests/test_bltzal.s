
# LB LH LW LBU
# LHU SB SH SW BLTZ BGEZ
# BLTZAL BGEZAL  

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
test_bltzal_take:
	addi $t0, $zero, -1
	add $t1, $zero, $ra       # Record the return address.
	bltzal $t0, bltzal_take_1 # Should return back here right away.
	beq $ra, $t1, inf         # Should have changed
	jal reset
	j bltzal_take_done_1
bltzal_take_1:
	jr $ra
bltzal_take_done_1:
test_bltzal_no_take:
	addi $t0, $zero, 5
	add $t1, $zero, $ra       # Record the return address.
	bltzal $t0, test_bltzal_not_taken
	beq $ra, $t1, inf         # Should not have changed
	j test_bltzal_done
test_bltzal_not_taken:
	jr $ra
test_bltzal_done:
	jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
