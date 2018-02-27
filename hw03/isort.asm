.globl main

.data
	prompt_length: .asciiz "Length of input: "
	prompt_num: .asciiz "Enter a number: "
	print_original_list: .asciiz "Original list: "
	print_sorted_list: .asciiz "Sorted list: "
	space: .asciiz " "
	eol: .asciiz "\n"
.text
main:
	# $t0 = n
	# $t1 = xs
	# $t2 = ii
	# $t3 = tmp
	# $t4 = 4
	# $t5 = ys
	
	# Store variables on stack
	subi $sp, $sp, 100
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	sw $t3, 16($sp)
	sw $t4, 20($sp)
	sw $t5, 24($sp)

	# prompt for length, $t0 = n
	li $v0, 4
	la $a0, prompt_length
	syscall
	
	li $v0, 5
	syscall
	move $t0, $v0
	
	# allocate space for array in the heap
	li $t4, 4
	mul $t3, $t0, $t4
		
	li $v0, 9
	move $a0, $t3
	syscall
	move $t1, $v0
	
	# call read_nums, gets the numbers needed from user
	move $a0, $t0
	move $a1, $t1
	jal read_nums

	# print original entered list
	move $a0, $t0
	move $a1, $t1
	jal print_nums
	
	# sort list, output sorted list to $v0
	move $a0, $t0
	move $a1, $t1
	jal isort
	
	# print sorted list
	move $a0, $t0
	move $a1, $t5
	jal print_nums
	
	# safe exit (0)
	li $v0, 10
	syscall
	
	# restore the stack (doesn't really matter for main, but done anyways
	lw $t5, 24($sp)
	lw $t4, 20($sp)
	lw $t3, 16($sp)
	lw $t2, 12($sp)
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 100
	jr $ra
###############################################################################
read_nums:
	# $t0 = n
	# $t1 = xs
	# $s0 = user_input
	# Store variables onto stack
	subi $sp, $sp, 12
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	
	# $t0 = n; $t1 = xs
	move $t0, $a0
	move $t1, $a1
	
	# ii = 0
	li $t2, 0
enter_nums:
	# Prompt for number
	li $v0, 4
	la $a0, prompt_num
	syscall
	
	# User enters number
	li $v0, 5
	syscall
	move $s0, $v0
	
	# Calculate the heap address (location of xs)
	li $t4, 4
	mul $t3, $t4, $t2 
	add $t3, $t1, $t3
	
	sw $s0, 0($t3)	# entered_list[index] = x
	
	addi $t2, $t2, 1 # i = i + 1
	
	blt $t2, $t0, enter_nums # check to see if n numbers have been entered
	
	# return xs
	move $v0, $t1
	
	# restore the stack
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	jr $ra
###############################################################################
print_nums:
	# $t0 = nn
	# $t1 = list
	
	# Store $t0 (nn) and $t1 (list) on stack
	subi $sp, $sp, 12
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	
	# Load arguments
	move $t0, $a0
	move $t1, $a1
	
	# ii = 0
	li $s0, 0
print_n:
	# $s0 = xs[ii]
	li $t4, 4
	mul $t3, $t4, $s0
	add $t3, $t3, $t1
	lw $a0, 0($t3)
	
	# prinft(xs[ii])
	li $v0, 1
	syscall
	
	li $v0, 4
	la $a0, space
	syscall
	
	# ii++
	addi $s0, $s0, 1
	# check for loop condition
	blt $s0, $t0, print_n
	
	li $v0, 4
	la $a0, eol
	syscall
	
	# Restore $t0 and $t1 before returning to main
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	jr $ra
###############################################################################
isort:
	# $t0 = nn
	# $t1 = xs
	# $t5 = ys
	
	# store arguments in the stack
	subi $sp, $sp, 12
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	
	# $t0 = nn; $t1 = xs
	move $t0, $a0
	move $t1, $a1
	
	# allocate space for ys on heap
	li $t4, 4
	mul $t3, $t0, $t4
		
	li $v0, 9
	move $a0, $t3
	syscall
	move $t5, $v0
		
	# ii = 0
	li $t2, 0
for_loop:
	# tmp = xs[ii]
	li $t4, 4
	mul $t3, $t4, $t2
	add $t3, $t1, $t3
	lw $t3, 0($t3)
	
	# $a0 = ys
	move $a0, $t5
	# $a1 = ii
	move $a1, $t2
	# $a2 = xs[ii]
	move $a2, $t3
	
	# insert(ys, ii, xs[ii])
	jal insert
	
	# check for loop
	addi $t2, $t2, 1
	blt $t2, $t0, for_loop 
	
	# return ys
	move $v0, $t5
	
	# restore the stack
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 12
	jr $ra
###############################################################################
insert:
	# $t0 = nn
	# $t1 = xs
	# $t2 = xx
	
	# store arguments on stack
	subi $sp, $sp, 16
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	
	# $t0 = nn
	move $t0, $a1
	# $t1 = xs
	move $t1, $a0
	# $t2 = xx
	move $t2, $a2
	
	# ii = 0 
	li $s0, 0
insert_for_loop:
	beq $t0, 0, end_insert_for_loop
	# yy = xs[ii]
	li $t4, 4
	mul $s1, $t4, $s0
	add $s1, $s1, $t1
	lw $s1, 0($s1)
	
	# if (xx < yy):
	blt $t2, $s1, if_statement
	
check_insert_for_loop:
	# ii++
	addi $s0, $s0, 1
	# check for loop (ii < nn)
	blt $s0, $t0, insert_for_loop

end_insert_for_loop:
	# xs[ii] = xx
	li $t4, 4
	mul $t3, $t4, $s0
	add $t3, $t3, $t1
	sw $t2, 0($t3)
	
	# restore the stack
	lw $t2, 12($sp)
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 16
	jr $ra
	
if_statement:
	# xs[ii] = xx
	li $t4, 4
	mul $s2, $t4, $s0 
	add $s2, $s2, $t1
	sw $t2, 0($s2)

	# xx = yy
	move $t2, $s1
	j check_insert_for_loop
	
	
	
	
	
	
	
	
	
	
	
	
	
	
