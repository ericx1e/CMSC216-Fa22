# strlen(str)

strlen:
   # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY
   li $v0, 0 # Init count to 0

loop:
   lb $t1, 0($a0) # t1 holds next char
   beqz $t1, ret
   addi $a0, $a0, 1
   addi $v0, $v0, 1 # increase count
   j loop


ret:
   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra
