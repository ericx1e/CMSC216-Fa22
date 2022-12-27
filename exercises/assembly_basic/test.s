    .data
x:
    .word 0

    .text
main:
    li $v0 5
    syscall
    sw $v0 x
    move $t0 $v0

    mul $t0 $t0 10

    li $v0 1
    move $a0 $t0
    syscall

    li $v0 11
    li $a0 10
    syscall

    jr $ra
    