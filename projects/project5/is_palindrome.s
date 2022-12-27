# Eric Xie
# 118862019
# exie
# Project 5

is_palindrome:
   # PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8
    
   # BODY
	jal strlen
	move $t1, $v0 # stores the length of the string in t1
	li $t0, 0 # t0 is our index

	div $t2, $t1, 2 # divide t1 (string length) by 2 and put it in t2

loop:
	bge $t0, $t2, yes # if we loop through half the string without returning no, yes palindrome
	move $t3, $a0 # first char
	add $t3, $t3, $t0 # increments by index to get char at i 
	lb $t6, 0($t3) # first char of incremented string 
	move $t4, $a0 # loads first char of a0 again

	sub $t5, $t1, $t0 # t5 = len - i 
	subu $t5 1 # t5 = len - i - 1
    
	add $t4, $t4, $t5 # getting char at len - i - 1
	lb $t7, 0($t4) # first char of incremented string

	bne $t6, $t7, no # the two characters aren't equal, so the string is not a palindrome
	add $t0, $t0, 1 # increase i
	j loop # start the loop again
no:
	li $v0, 0 # returns 0
	j ret
yes:	
	li $v0, 1 # returns 1
ret:
   # EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra


# strlen(str)

strlen:
   # PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8
   # BODY
	li $t0, 0 # loads $t0 with number 0, this is the index
	move $t2, $a0 # moves a0 to t2 so we don't modify the original argument
	lb $t1, 0($t2) # loads the first character of the string

loopsl:
	lb $t1, 0($t2) # loads in the character at the current index
	beqz $t1, retsl # if $t1 is null, go to reutrn
	add $t0, $t0, 1 # increases the counter
	add $t2, $t2, 1 # increases the index of string
	j loopsl # starts the loop again

retsl:
	move $v0, $t0 # moves the counter, t0 to v0
   # EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp) 
	jr $ra 
