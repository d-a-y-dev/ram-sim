# THIS TEST FILE ASSUMES THAT THE DATA IS SET.

.text

main:   
	addi $t0, $t0, 5
	lw $t0, 0($0)
exit:
        addiu $v0, $zero, 0xa
        syscall
