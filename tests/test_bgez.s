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
test_bgez_take_1:
	bgez $t0, bgez_take_1
	j inf
bgez_take_1:
	jal reset
test_bgez_take_2:
	addi $t0, $zero, 256
	bgez $t0, bgez_take_2
	j inf
bgez_take_2:
	jal reset
test_bgez_no_take_1:
	addi $t0, $zero, -1
	bgez $t0, inf
	jal reset
test_bgez_no_take_2:
	addi $t0, $zero, -256
	bgez $t0, inf
	jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
