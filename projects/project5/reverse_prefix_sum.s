# Eric Xie
# 118862019
# exie
# Project 5

reverse_prefix_sum:
   # PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8

   # BODY
	subu $sp, $sp, 4 # grows stack for *arr
	lw $t0, 0($a0) # loads *arr
	sw $t0, 4($sp) # saves *arr onto the stack
	beq $t0, -1, zero # base case, return zero

	add $a0, $a0, 4 # arr + 1
	jal reverse_prefix_sum # recursion with updated a0
	lw $t0, 4($sp) # recovers curr index of arr
	move $t1, $v0 # get return value of reverse_prefix_sum(arr+1)
	addu $t2, $t1, $t0 # calculates r, stores in t2
	sub $a0, $a0, 4 # returns arr to its original index
	sw $t2, 0($a0) # *arr = r
	move $v0, $t2 # move r to return register
    j ret # return
zero:
	li $v0, 0 # returns 0
ret:
    addu $sp, $sp, 4 # recover stack
   # EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra