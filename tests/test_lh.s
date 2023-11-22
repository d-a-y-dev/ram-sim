

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
test_lh:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	lui $t2, 0xffff
	ori $t2, $t2, 0xabcd

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lh $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lh_extend:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xab00
	lui $t2, 0xffff
	ori $t2, $t2, 0xab00

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lh $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lh_no_extend:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0x7b00
	ori $t2, $zero, 0x7b00

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lh $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lh_shift:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $t2, 0xab

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lh $t1, 1($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
