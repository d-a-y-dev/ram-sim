

# This test assumes that all jump and branching instruction works.

.text

main:   
        j exit                                 # 0x00
        addi $t0, $zero, 5                    # 0x04
        addi $t1, $zero, 10                   # 0x10
exit:
        addi $v0, $zero, 0xa                  # 0x14
        syscall
