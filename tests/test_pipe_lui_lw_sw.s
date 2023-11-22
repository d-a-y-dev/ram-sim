.text

main:   
    lui $t1, 0x1000
    addi $t0, $zero, 10
    sw $t0, 0($t1)
    lw $v0, 0($t1)
exit:
        syscall
