





# This test assumes that all jump and branching instruction works.

.text

main:   
	ori $t0, $zero, 0xbeef
	ori $t1, $zero, 0xabcd
exit:
        addiu $v0, $zero, 0xa
        syscall
