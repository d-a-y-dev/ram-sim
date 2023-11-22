# This test assumes that all jump and branching instruction works.

.text

main:   
        jal exit                                 # 0x00
        addiu $t0, $zero, 5                    # 0x04
        addiu $t1, $zero, 10                   # 0x10
exit:
        addiu $v0, $zero, 0xa                  # 0x14
        syscall
