
.text

main:   
        addiu $t0, $zero, 2
        addiu $t1, $zero, -1
        addu $v1, $t0, $t1
        addi $v0, $zero, 0x0a
        syscall
