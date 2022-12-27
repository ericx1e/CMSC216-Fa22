# Eric Xie
# 118862019
# exie
# Project 5

isqrt:
   # PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8
	
   # BODY
	blt $a0, 2, retn # if a0 < 2, return n
	
	subu $sp, $sp, 4 # grows stack for n
	sw $a0, 4($sp) # saves original n to stack
	
	srl $a0, $a0, 2 # n right bitshifts twice (n >> 2)
	jal isqrt # calls isqrt with new n

	lw $a0, 4($sp) # restores original n
	add $sp, $sp, 4 # restores stack
	
	move $t0, $v0 # moves return value of isqrt(n >> 2) to t0
	sll $t0, $t0, 1 # result bitshift left once to calculate small
	add $t1, $t0, 1 # calculate large (small + 1)
	mul $t2, $t1, $t1 # large * large

	bgt $t2, $a0, retsmall # (large * large > n), return small
retlarge:
	move $v0, $t1 # returns large
	j ret # jump to end
retn:
	move $v0, $a0 # returns n
	j ret # jump to end
retsmall:
	move $v0, $t0 # returns small
	
ret:
   # EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra