.globl main

.data
	promptop: .asciiz "Please enter an operator: "
	promptnum: .asciiz "Please enter an integer: "
	endline: .asciiz "\n"
	answer: .asciiz "The result is "

.text 
main:	
	# Read in operator
    	la $a0, promptop    
      	li $v0, 4
      	syscall
      	
      	li $v0, 12
      	syscall
      	move $t0, $v0
      	
      	la $a0, endline
      	li $v0, 4
      	syscall
      	
      	# Read in first number
      	la $a0, promptnum
      	li $v0, 4
      	syscall
      	
      	li $v0, 5
      	syscall
      	move $t1, $v0
      	
      	la $a0, endline
      	li $v0, 4
      	syscall
      	
      	# Read in second number
      	la $a0, promptnum
      	li $v0, 4
      	syscall
      	
      	li $v0, 5
      	syscall
      	move $t2, $v0
      	
      	la $a0, endline
      	li $v0, 4
      	syscall
      	
      	# Branch based on input character, then call jal
      	la $ra, exit_main
      	beq $t0, 42, mulnums
      	beq $t0, 43, addnums
      	beq $t0, 45, subnums
      	beq $t0, 47, divnums
exit_main:
      	# Print result and exit (0)
      	move $t4, $v0
      	la $a0, answer
	li $v0, 4
	syscall
	
	li $v0, 1
	move $a0, $t4
	syscall
	
	la $a0, endline
	li $v0, 4
	syscall
	
	li $v0, 10
	syscall
      	
addnums:
	add $v0, $t1, $t2
	jr $ra
	
subnums:
	sub $v0, $t1, $t2
	jr $ra
	
mulnums:
	mul $v0, $t1, $t2
	jr $ra
	
divnums:
	div $v0, $t1, $t2
	jr $ra
	
	
	
