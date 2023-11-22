.text
main:
    addi $t0 $zero 0x0a
    addi $t0 $t0 -1
    mthi $t0
    addi $v0 $zero 0x0a
    syscall
    
