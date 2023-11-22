# LH
# LHU  

# This test assumes that all jump and branching instruction works. It also assumes that LW works

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
test_addi_pos:
                                      # test ADDI positive
        addi $t0, $t0, 0x1a
        ori $t1, $t1, 0x1a
        bne $t0, $t1, inf
        jal reset
test_addi_neg:
                                      # test ADDI negative
        addi $t0, $t0, -1
        lui $t1, 0xFFFF
        ori $t1, $t1, 0xFFFF
        bne $t0, $t1, inf
        jal reset
test_add_pos:
                                      # test add positive
        ori $t1, $t1, 0x1a
        add $t0, $t0, $t1
        bne $t0, $t1, inf
        jal reset
test_add_neg:
                                      # test add negative
        addi $t2, $t2, -1
        add $t0, $t0, $t2
        lui $t1, 0xFFFF
        ori $t1, $t1, 0xFFFF
        bne $t0, $t1, inf
        jal reset
test_addu_pos:
                                      # test addu positive
        ori $t1, $t1, 0x1a
        addu $t0, $t0, $t1
        bne $t0, $t1, inf
        jal reset
test_addu_neg:
                                      # test addu negative
        addi $t2, $t2, -1
        addu $t0, $t0, $t2
        lui $t1, 0xFFFF
        ori $t1, $t1, 0xFFFF
        bne $t0, $t1, inf
        jal reset
test_addiu_pos:
                                      # test ADDIU positive
        addiu $t0, $t0, 0x1a
        ori $t1, $t1, 0x1a
        bne $t0, $t1, inf
        jal reset
test_addiu_neg:
                                      # test ADDIU negative
        addiu $t0, $t0, -1
        lui $t1, 0xFFFF
        ori $t1, $t1, 0xFFFF
        bne $t0, $t1, inf
        jal reset
test_sub:
                                      # test sub (pos result)
        ori $t0, $t0, 0xFF
        ori $t1, $t1, 0x4
        sub $t3, $t0, $t1
        ori $t4, $t4, 0xFB
        bne $t3, $t4, inf
        jal reset
test_sub_neg:
                                      # test sub (neg result)
        ori $t0, $t0, 0x0
        ori $t1, $t1, 0x4
        sub $t3, $t0, $t1
        lui $t4, 0xFFFF
        ori $t4, $t4, 0xFFFC
        bne $t3, $t4, inf
        jal reset
test_sub_neg_1:
                                      # test sub (neg result)
        ori $t0, $t0, 0x4
        ori $t1, $t1, 0x0
        sub $t3, $t0, $t1
        addi $t4, $zero, 4
        bne $t3, $t4, inf
        jal reset
 test_subu:
                                      # test subu (pos result)
         ori $t0, $t0, 0xFF
         ori $t1, $t1, 0x4
         subu $t3, $t0, $t1
         ori $t4, $t4, 0xFB
         bne $t3, $t4, inf
         jal reset
test_subu_neg:
                                      # test subu (neg result)
        ori $t0, $t0, 0x0
        ori $t1, $t1, 0x4
        subu $t3, $t0, $t1
        lui $t4, 0xFFFF
        ori $t4, $t4, 0xFFFC
        bne $t3, $t4, inf
        jal reset
test_subu_neg_1:
                                      # test subu (neg result)
        ori $t0, $t0, 0x4
        ori $t1, $t1, 0x0
        subu $t3, $t0, $t1
        addi $t4, $zero, 4
        bne $t3, $t4, inf
        jal reset
test_and_00:
        addi $t0, $zero, 0
        addi $t1, $zero, 0
        and $t2, $t0, $t1
        bne $t2, $zero, inf
        jal reset
test_and_01:
        addi $t0, $zero, 0
        addi $t1, $zero, 1
        and $t2, $t0, $t1
        bne $t2, $zero, inf
        jal reset
test_and_11:
        addi $t0, $zero, 1
        addi $t1, $zero, 1
        and $t2, $t0, $t1
        beq $t2, $zero, inf
        jal reset
test_and_10:
        addi $t0, $zero, 1
        addi $t1, $zero, 0
        and $t2, $t0, $t1
        bne $t2, $zero, inf
        jal reset
test_andi_00:
        addi $t0, $zero, 0
        andi $t2, $t0, 0
        bne $t2, $zero, inf
        jal reset
test_andi_01:
        addi $t0, $zero, 0
        andi $t2, $t0, 1
        bne $t2, $zero, inf
        jal reset
test_andi_11:
        addi $t0, $zero, 1
        andi $t2, $t0, 1
        beq $t2, $zero, inf
        jal reset
test_andi_10:
        addi $t0, $zero, 1
        andi $t2, $t0, 0
        bne $t2, $zero, inf
        jal reset
test_or_00:
        addi $t0, $zero, 0
        addi $t1, $zero, 0
        or $t2, $t0, $t1
        bne $t2, $zero, inf
        jal reset
test_or_01:
        addi $t0, $zero, 0
        addi $t1, $zero, 1
        or $t2, $t0, $t1
        beq $t2, $zero, inf
        jal reset
test_or_11:
        addi $t0, $zero, 1
        addi $t1, $zero, 1
        or $t2, $t0, $t1
        beq $t2, $zero, inf
        jal reset
test_or_10:
        addi $t0, $zero, 1
        addi $t1, $zero, 0
        or $t2, $t0, $t1
        beq $t2, $zero, inf
        jal reset
test_xor_00:
        addi $t0, $zero, 0
        addi $t1, $zero, 0
        xor $t2, $t0, $t1
        bne $t2, $zero, inf
        jal reset
test_xor_01:
        addi $t0, $zero, 0
        addi $t1, $zero, 1
        xor $t2, $t0, $t1
        beq $t2, $zero, inf
        jal reset
test_xor_11:
        addi $t0, $zero, 1
        addi $t1, $zero, 1
        xor $t2, $t0, $t1
        bne $t2, $zero, inf
        jal reset
test_xor_10:
        addi $t0, $zero, 1
        addi $t1, $zero, 0
        xor $t2, $t0, $t1
        beq $t2, $zero, inf
        jal reset
test_xori_00:
        addi $t0, $zero, 0
        xori $t2, $t0, 0
        bne $t2, $zero, inf
        jal reset
test_xori_01:
        addi $t0, $zero, 0
        xori $t2, $t0, 1
        beq $t2, $zero, inf
        jal reset
test_xori_11:
        addi $t0, $zero, 1
        xori $t2, $t0, 1
        bne $t2, $zero, inf
        jal reset
test_xori_10:
        addi $t0, $zero, 1
        xori $t2, $t0, 0
        beq $t2, $zero, inf
        jal reset
test_nor_00:
        addi $t0, $zero, 0
        addi $t1, $zero, 0
        addi $t3, $zero, -1
        nor $t2, $t0, $t1
        bne $t2, $t3, inf
        jal reset
test_nor_01:
        addi $t0, $zero, 0
        addi $t1, $zero, 1
        addi $t3, $zero, -2
        nor $t2, $t0, $t1
        bne $t2, $t3, inf
        jal reset
test_nor_11:
        addi $t0, $zero, 1
        addi $t1, $zero, 1
        addi $t3, $zero, -2
        nor $t2, $t0, $t1
        bne $t2, $t3, inf
        jal reset
test_nor_10:
        addi $t0, $zero, 1
        addi $t1, $zero, 0
        addi $t3, $zero, -2
        nor $t2, $t0, $t1
        bne $t2, $t3, inf
        jal reset
test_mthi_mfhi:
        addi $t0, $zero, 0xbef
        mthi $t0
        mfhi $t2
        bne $t0, $t2, inf
        mthi $zero
        jal reset
test_mtlo_mflo:
        addi $t0, $zero, 0xded
        mtlo $t0
        mflo $t2
        bne $t0, $t2, inf
        mtlo $zero
        jal reset
test_mult_pos:
        addi $t0, $t0, 333        
        addi $t1, $t1, 111
        mult $t1, $t0
        mfhi $t2
        mflo $t3
        ori $t4, $t4, 0x9063
        bne $t2, $zero, inf
        bne $t3, $t4, inf
        jal reset
test_mult_neg:
        addi $t0, $t0, 333        
        addi $t1, $t1, -111
        mult $t1, $t0
        mfhi $t2
        mflo $t3
        addi $t1, $zero, -1

        addi $t4, $zero, 0
        lui $t4, 0xffff
        ori $t4, $t4, 0x6f9d
        bne $t2, $t1, inf
        bne $t3, $t4, inf
        jal reset
test_multu_pos:
        addi $t0, $t0, 333        
        addi $t1, $t1, -111
        multu $t1, $t0
        mfhi $t2
        mflo $t3
        ori $t1, $zero, 0x14c         # testing hi
        bne $t2, $t1, inf
        lui $t4, 0xffff               # testing lo
        ori $t4, $t4, 0x6f9d
        bne $t3, $t4, inf
        jal reset
test_div_pos:
        ori $t0, $t0, 421
        ori $t1, $t1, 107
        div $t0, $t1
        mfhi $t2
        mflo $t3
        addi $t0, $zero, 3
        ori $t1, $zero, 0x64
        bne $t3, $t0, inf
        bne $t2, $t1, inf
        jal reset
test_div_neg:
        addi $t0, $t0, 421
        addi $t1, $t1, -107
        div $t0, $t1
 
        mflo $t2
        addi $t1, $zero, 0
        lui $t1, 0xffff
        ori $t1, $t1, 0xfffd
        bne $t2, $t1, inf
 
        mfhi $t3
        addi $t0, $zero, 0x64
        bne $t3, $t0, inf
        jal reset
test_divu_pos:
        ori $t0, $t0, 421
        ori $t1, $t1, 107
        divu $t0, $t1
        mfhi $t2
        mflo $t3
        addi $t0, $zero, 3
        ori $t1, $zero, 0x64
        bne $t3, $t0, inf
        bne $t2, $t1, inf
        jal reset
test_divu_neg:
        ori $t0, $t0, 421
        addi $t1, $t1, -107
        divu $t0, $t1
        mfhi $t2
        mflo $t3
        addi $t0, $zero, 3
        bne $t3, $zero, inf
        ori $t1, $zero, 0x1a5
        bne $t2, $t1, inf
        jal reset
test_slt0:
        addi $t0, $zero, -1           # t0 = -1
        addi $t2, $zero, -1           # t2 = -1
        slt $t1, $t0, $t2             # t1 = 0 
        bne $t1, $zero, inf   
        jal reset
test_slt1:
        addi $t0, $zero, -1           # t0 = -1
        addi $t2, $zero, 1            # t2 = 1
        slt $t1, $t0, $t2             # t1 = 1 
        beq $t1, $zero, inf   
        jal reset
test_sltu0:
        addi $t0, $zero, -1           # t0 = -1
        addi $t2, $zero, 1            # t2 = 1
        sltu $t1, $t0, $t2            # t1 = 0 
        bne $t1, $zero, inf
        jal reset
test_sltu1:
        addi $t0, $zero, -1           # t0 = -1
        addi $t2, $zero, -2           # t2 = 1
        sltu $t1, $t2, $t1            # t1 = 1
        bne $t1, $zero, inf   
        jal reset
test_slti0:
        addi $t0, $zero, -1           # t0 = -1
        slti $t1, $t0, -1             # t1 = 0 
        bne $t1, $zero, inf   
        jal reset

        addi $t0, $zero, -1           # t0 = -1
        slti $t1, $t0, 1              # t1 = 1 
        beq $t1, $zero, inf           # if t1 == 0, fail
        jal reset
test_lui:
        lui $t0, 0xffff
        ori $t0, $t0, 0xffff
        addi $t1, $t1, -1
        bne $t0, $t1, inf
        jal reset
test_sltiu0:
        addi $t0, $zero, -1           # t0 = -1
        sltiu $t1, $t0, 1             # t1 = 0 
        bne $t1, $zero, inf   
        jal reset
test_sltiu1:
        addi $t0, $zero, 1            # t0 = 1
        sltiu $t1, $t0, -1            # t1 = 0
        addi $t2, $zero, 1
        bne $t1, $t2, inf   
        jal reset
test_srlv:
        lui $t3, 0x1fff
        ori $t3, 0xfff3
        addi $t0, $zero, -100         # t0 -> -100
        addi $t1, $zero, 35           # t1 -> 35
        srlv $t2, $t0, $t1            # t2 -> (-100) >> 3
        bne $t2, $t3, inf    
        jal reset
test_srav:
        addi $t3,  $zero, -13         # t3 -> -13
        addi $t0,  $zero, -100        # t0 -> -100
        addi $t1,  $zero, 35          # t1 -> 35
        srav $t2,  $t0, $t1           # t2 -> (-100) >> 3
        bne $t2,  $t3, inf            # Jump to exit if $t2 = -13
        jal reset
test_sll:
        addi $t0, $zero, 1            # t0 = 1
        addi $t1, $zero, 8            # t1 = 8
        sll $t2, $t0, 3               # t2 = 8
        bne $t2, $t1, inf
        jal reset
test_sllv:
        addi $t0,  $zero, 35          # t0
        addi $t1,  $zero, 1           # t0
        sllv $t3,  $t1  , $t0         # t3 = 8
        addi $t3,  $t3  , 2
        addi $t2,  $zero, 10
        bne  $t2,  $t3  , inf
        jal reset
test_srl:
        addi $t0, $zero, 32           # t0 = 1
        srl $t2, $t0, 2               # v0 = 8
        addi $t1, $zero, 8
        bne $t1, $t2, inf
        jal reset
test_jalr:
        addi $t3, $zero, 5
        jal jump_label_1
        jal jump_label_2
        j jump_exit_label
jump_label_1:
        add $t0, $zero, $ra
        beq $t2, $t3, jump_okay
        jr $ra
jump_label_2:
        jr $ra
jump_exit_label:
        addi $t1, $t1, 4
        sub $ra, $ra, $t1
        bne $ra, $t0, inf
        addi $t2, $zero, 5
        addi $t0, $t0, 8
        jalr $t0
jump_okay:
        jal reset
test_blez_not_taken:
        addi $t0, $zero, 100
        blez $t0, inf
        j blez_done
blez_done:
        jal reset
test_blez_taken_1:
	blez $t0 blez_take_zero
	j inf
blez_take_zero:
	jal reset
test_blez_taken_2:
	addi $t0, $zero, -25
	blez $t0 blez_take_neg
	j inf
blez_take_neg:
	jal reset
test_bgtz_not_taken:
        bgtz $t0, inf
        jal reset
test_bgtz_taken_1:
	addi $t0, $zero, 5
	bgtz $t0 bgtz_take_5
	j inf
bgtz_take_5:
	jal reset
test_bgtz_not_taken_2:
	addi $t0, $zero, -1
        bgtz $t0, inf
        jal reset
test_bltzal_take:
	addi $t0, $zero, -1
	add $t1, $zero, $ra       # Record the return address.
	bltzal $t0, bltzal_take_1 # Should return back here right away.
	beq $ra, $t1, inf         # Should have changed
	jal reset
	j bltzal_take_done_1
bltzal_take_1:
	jr $ra
bltzal_take_done_1:
test_bltzal_no_take:
	addi $t0, $zero, 5
	add $t1, $zero, $ra       # Record the return address.
	bltzal $t0, test_bltzal_not_taken
	beq $ra, $t1, inf         # Should not have changed
	j test_bltzal_done
test_bltzal_not_taken:
	jr $ra
test_bltzal_done:
	jal reset
test_bgezal_take_1:
	add $t1, $zero, $ra       # Record the return address.
	bgezal $t0, bgezal_take_1 # Should return back here right away.
	beq $ra, $t1, inf         # Should have changed
	jal reset
	j bgezal_take_done_1
bgezal_take_1:
	jr $ra
bgezal_take_done_1:
test_bgezal_take_2:
	addi $t1, $zero, 69
	add $t1, $zero, $ra       # Record the return address.
	bgezal $t0, bgezal_take_2 # Should return back here right away.
	beq $ra, $t1, inf         # Should have changed
	jal reset
	j bgezal_take_done_2
bgezal_take_2:
	jr $ra
bgezal_take_done_2:
test_bgezal_no_take:
	addi $t0, $zero, -4
	add $t1, $zero, $ra       # Record the return address.
	bgezal $t0, test_bgezal_not_taken
	beq $ra, $t1, inf         # Should not have changed
	j test_bgezal_done
test_bgezal_not_taken:
	jr $ra
test_bgezal_done:
	jal reset
test_sra_neg:
	addi $t0, $zero, 3
	addi $t1, $zero, -16
	sra $t2, $t1, $t0
	addi $t0, $zero, -2
	bne $t0, $t2 inf
	jal reset
test_sra_neg_1:
	addi $t0, $zero, 3
	addi $t1, $zero, -69
	sra $t2, $t1, $t0
	addi $t0, $zero, -9
	bne $t0, $t2 inf
	jal reset
test_sra_pos:
	addi $t0, $zero, 3
	addi $t1, $zero, 16
	sra $t2, $t1, $t0
	addi $t0, $zero, 2
	bne $t0, $t2 inf
	jal reset
test_bltz_take_1:
	addi $t0, $zero, -1
	bltz $t0, bltz_take_1
	j inf
bltz_take_1:
	jal reset
test_bltz_take_2:
	addi $t0, $zero, -25
	bltz $t0, bltz_take_2
	j inf
bltz_take_2:
	jal reset
bltz_no_take_1:
	bltz $t0, inf
	jal reset
bltz_no_take_2:
	addi $t0, $zero, 256
	bltz $t0, inf
	jal reset
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
test_sw:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, 255

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lw $t1, 0($3)

	# Should be the same
	bne $t1, $t0, inf
	sw $zero, 0($3)
	jal reset
test_sw_offset:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, 255

	# Save in memory
	sw $t0, 4($3)

	# Load data back into $t1
	lw $t1, 4($3)

	# Should be the same
	bne $t1, $t0, inf
	sw $zero, 4($3)
	jal reset
test_sw_offset_half:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, -1
	ori $t2, 0xffff

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lw $t1, 2($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
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
test_sh:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $zero, 0xabcd

	# Save in memory
	sh $t0, 0($3)

	# Load data back into $t1
	lw $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sh $zero, 0($3)
	jal reset
test_sh_offset:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, 255

	# Save in memory
	sh $t0, 4($3)

	# Load data back into $t1
	lw $t1, 4($3)

	# Should be the same
	bne $t1, $t0, inf
	sh $zero, 4($3)
	jal reset
test_sh_offset_half:
	lui $3, 0x1000

	# Set values
	addi $t0, $zero, -1

	# Save in memory
	sh $t0, 0($3)

	# Load data back into $t1
	lw $t1, 2($3)

	# Should be the same
	bne $t1, $zero, inf
	sh $zero, 0($3)
	jal reset
test_sh_offset_fourth:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xbeef
	ori $t2, $zero, 0xbe

	# Save in memory
	sh $t0, 0($3)

	# Load data back into $t1
	lw $t1, 1($3)

	# Should be the same
	bne $t1, $t2, inf
	sh $zero, 0($3)
	jal reset
test_lb:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	lui $t2, 0xffff
	ori $t2, $t2, 0xffcd

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lb $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lb_none:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xab00

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lb $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lb_shift:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	lui $t2, 0xffff
	ori $t2, $t2, 0xffab

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lb $t1, 1($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lbu:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $zero, 0x00cd

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lbu $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lbu_none:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xab00

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lbu $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lbu_shift:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $zero, 0xab

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lbu $t1, 1($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
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
test_lhu:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $t2, 0xabcd

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lhu $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lhu_extend_ignored:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xab00
	ori $t2, $t2, 0xab00

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lhu $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lhu_no_extend:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0x7b00
	ori $t2, $zero, 0x7b00

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lhu $t1, 0($3)

	# Should be the same
	bne $t1, $t2, inf
	sw $zero, 0($3)
	jal reset
test_lhu_shift:
	lui $3, 0x1000

	# Set values
	ori $t0, $zero, 0xabcd
	ori $t2, $t2, 0xab

	# Save in memory
	sw $t0, 0($3)

	# Load data back into $t1
	lhu $t1, 1($3)

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
