.globl main

.data
	prompt: .asciiz "Please enter a number: "
	endline: .asciiz "\n"
	answer: .asciiz "The result is "

.text 
main:
	# Prompt 
	li $v0, 4
	la $a0, prompt
	syscall
	
	li $v0, 5
	syscall
	move $a0, $v0
	li $v0, 0
	
	jal fib
	
	move $a0, $v0
	li $v0, 1	
	syscall
	
	li $v0, 10	
	syscall
	

fib:
	# fib(n):
	# if n == 0:
	# 	return 0
	# elif n == 1:
	#	return 1
	# else
	# 	a = fib(n - 1)
	#	b = fib(n - 2)
	# 	return a + b
	# Push ra, n, n-1 and n-2 onto stack
	subi $sp, $sp, 20
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	
	move $t0, $a0
	subi $t1, $t0, 1
	subi $t2, $t0, 2
	
	# n > 1
	bgt $t0, 1, fib_else
	
	# n == 0
	beq $t0, 1, one_base_case

	# n == 1
	j fib_return
	
fib_else:
	subi $a0, $t0, 1
	jal fib
	subi $a0, $t0, 2
	jal fib


fib_return:
	lw $t2, 12($sp)
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 20
	jr $ra

one_base_case:
	addi $v0, $v0, 1
	j fib_return
