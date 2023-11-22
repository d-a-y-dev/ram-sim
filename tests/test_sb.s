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
test_sb:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $zero, 0xcd

	# Save in memory
	sb $t0, 0($3)

	# Load data back into $t1
	lw $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sb $zero, 0($3)
	jal reset
test_sb_offset:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, 255

	# Save in memory
	sb $t0, 4($3)

	# Load data back into $t1
	lw $t1, 4($3)

	# Should be the same
	bne $t1, $t0, inf
	sb $zero, 4($3)
	jal reset
test_sb_offset_half:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, -1

	# Save in memory
	sb $t0, 0($3)

	# Load data back into $t1
	lw $t1, 2($3)

	# Should be the same
	bne $t1, $zero, inf
	sb $zero, 0($3)
	jal reset
done:
        j exit
inf:
        addi $t7, $t7, -1
        j exit
exit:
        addiu $v0, $zero, 0xa
        syscall
