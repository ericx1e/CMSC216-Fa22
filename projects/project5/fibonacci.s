# Eric Xie
# 118862019
# exie
# Project 5

fibonacci:
   # PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8
    
   # BODY
	bgt $a0, 2, rec # recursion if n >= 2
	li $v0, 1 # base case, returns 1
    j ret

rec:
	subu $sp, $sp, 4
	sw $a0, 4($sp) # grows stack and saves a0 
	sub $a0, $a0, 1 # subtract one from a0 for next fib call
	jal fibonacci # calls fib with (n - 1)
	lw $a0, 4($sp) # recovers a0 to origial value
	subu $sp, $sp, 4 # grows stack for return value of recursive call
	sw $v0, 4($sp) # saves result of fib (n - 1)	
	
	sub $a0, $a0, 2
	jal fibonacci # calls fib (n - 2)
	lw $t0, 4($sp) # get value of fib (n - 1)
	add $v0, $v0, $t0 # adds fib(n - 1) to fib(n - 2)
	addu $sp, $sp, 8 # restores the stack

ret:	
   # EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra 