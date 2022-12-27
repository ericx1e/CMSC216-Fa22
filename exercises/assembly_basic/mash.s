# mash(x, y)

mash:
   # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY
   li $v0, 0
   li $t3, 10
   mul $t0, $t0, $t3
   add $v0, $v0, $t0
   add $v0, $v0, $t1

   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra