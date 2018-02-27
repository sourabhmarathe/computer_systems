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
	# $t2 = ys
	# $t3 = tmp
	# $t4 = 4
	subi $sp, $sp, 28
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	sw $t3, 16($sp)
	sw $t4, 20($sp)
	
	# prompt for length, $t0 = n
	li $v0, 4
	la $a0, prompt_length
	syscall
	
	li $v0, 5
	syscall
	move $t0, $v0
	
	# allocate space for entered array in the heap
	li $t4, 4
	mul $t3, $t0, $t4
		
	li $v0, 9
	move $a0, $t3
	syscall
	move $t1, $v0
	
	# call read_nums, gets the numbers needed from user
	move $a0, $t0 	# n
	move $a1, $t1	# xs
	jal read_nums
	
	# allocate space for sorted array in the heap
	li $t4, 4
	mul $t3, $t0, $t4
	
	li $v0, 9
	move $a0, $t3
	syscall
	move $t2, $v0

	# print original entered list
	li $v0, 4
	la $a0, print_original_list
	syscall
	
	move $a0, $t0
	move $a1, $t1
	jal print_nums
	
	# call mergesort function
	move $a0, $t2	# ys = $t2
	li $a1, 0	# iBegin = 0
	move $a2, $t0	# iEnd = $t0
	move $a3, $t1	# xs = $t1
	jal merge
	
	# print sorted list
	li $v0, 4
	la $a0, print_sorted_list
	syscall
	
	mul $t7, $t4, $t0
	add $t7, $t1, $t7
	move $a0, $t0
	move $a1, $t7
	jal print_nums
	
	# safe exit (0)
	li $v0, 10
	syscall
	
	# restore stack
	lw $ra, 0($sp)
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 16($sp)
	lw $t4, 20($sp)
	addi $sp, $sp, 28
	
	
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
	
	li $t2, 0	# ii = 0
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
	
	sw $s0, 0($t3)		# entered_list[index] = x
	
	addi $t2, $t2, 1 	# ii = ii + 1
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
copy_array:
	# copies content of array A into array B
	# $t0 = A
	# $t1 = B
	# $t2 = n
	# $s0 = x
	# $t5 = tmp
	
	# store inputs on the stack
	subi $sp, $sp, 24
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	sw $s0, 16($sp)
	sw $t5, 20($sp)
	
	# load arguments
	move $t0, $a0
	move $t1, $a1
	move $t2, $a2
	
	li $t3, 0	# ii = 0
array_copy_loop:
	# x = A[ii]
	li $t4, 4
	mul $t5, $t3, $t4
	add $t5, $t5, $t1
	lw $s0, 0($t5)
	
	# B[ii] = x
	li $t4, 4
	mul $t5, $t3, $t4
	add $t5, $t5, $t0
	sw $s0, 0($t5)
	
	addi $t3, $t3, 1
	blt $t3, $t2, array_copy_loop
	
	lw $t5, 20($sp)
	lw $s0, 16($sp)
	lw $t2, 12($sp)
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	add $sp, $sp, 24
	jr $ra

###############################################################################
merge:
	# $t0 = ys
	# $t1 = xs
	# $t2 = iBegin
	# $t3 = iEnd
	
	# store registers on stack
	subi $sp, $sp, 20
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	sw $t3, 16($sp)
	
	# load variables from argument registers
	move $t0, $a0  
	move $t1, $a3
	move $t2, $a1
	move $t3, $a2
	
	# copy user entered array into sorted array
	move $a0, $t0
	move $a1, $t1
	move $a2, $t3
	jal copy_array
	
	# $t0 = B
	# $t1 = A
	# $t2 = iBegin
	# $t3 = iEnd
	move $a0, $t0
	move $a1, $t1
	move $a2, $t2
	move $a3, $t3
	jal split_merge
	
	lw $t3, 16($sp)
	lw $t2, 12($sp)
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 20
	jr $ra

###############################################################################
split_merge:
	# $t0 = ys
	# $t1 = xs
	# $t2 = iBegin
	# $t3 = iEnd
	subi $sp, $sp, 24
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	sw $t3, 16($sp)
	
	# load arguments
	move $t0, $a0
	move $t1, $a1
	move $t2, $a2
	move $t3, $a3
	
	# if (iEnd - iBegin) < 2, return
	sub $s0, $t3, $t2
	blt $s0, 2, end_split_loop
	
	li $v0, 1
	move $a0, $s0
	syscall
	
	
	# $s0 (iMiddle) = iEnd + iBegin/2
	add $s1, $t2, $t3
	li $s2, 2
	div $s0, $s1, $s2
	
	# recursively call split merge for both halves
	move $a0, $t0
	move $a1, $t1
	move $a2, $t2
	move $a3, $s0
	jal split_merge
	
	move $a0, $t0
	move $a1, $t1
	move $a2, $s0
	move $a3, $t3
	jal split_merge
	
	move $a0, $t2
	move $a1, $t3
	move $a2, $s0
	jal sort_merge

end_split_loop:
	move $v0, $t1

	lw $ra, 0($sp)
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 16($sp)
	addi $sp, $sp 24
	jr $ra
	
###############################################################################
sort_merge:
	# $t2 = iBegin
	# $t3 = iEnd
	# $s0 = iMiddle
	
	# store arguments and other important registers on stack (ys/xs)
	subi $sp, $sp, 24
	sw $ra, 0($sp)
	sw $t0, 4($sp)
	sw $t1, 8($sp)
	sw $t2, 12($sp)
	sw $t3, 16($sp)
	sw $s0, 20($sp)
	
	# load in arguments
	move $t2, $a0
	move $t3, $a1
	move $s0, $a2
	# i ($s2) = iBegin, j ($s3) = iEnd
	move $s2, $t2
	move $s3, $t3
	
	# kk = 0
	move $s1, $t2
loop_to_end:
	beq $s1, $t3, end_loop_to_end
	
	# $s7 = (i < iMiddle && (j >= iEnd || A[i] <= A[j]))
	# if i < iMiddle is false, go to else state
	bgt $s2, $s0, else_state
	beq $s2, $s0, else_state
	
	# compute j >=iEnd ($s4) || A[i] <= A[j] ($s5)
	bgt $s3, $t3, if_state_true
	beq $s3, $t3, if_state_true
	
	# $s3 = A[i]
	# $s4 = A[j]
	li $s5, 4
	mul $s3, $s5, $s2
	add $s3, $s3, $t0
	lw $s3, 0($s3)
	
	mul $s4, $s5, $s3
	add $s4, $s4, $t0
	lw $s4, 0($s4)
	
	# if A[i] <= A[j], if_state_true
	blt $s3, $s4, if_state_true
	beq $s3, $s4, if_state_true
	
	j else_state
	
if_state_true:
	# ys[k] = A[i]
	li $s5, 4
	mul $s4, $s1, $s5
	add $s4, $s4, $t1
	lw $s3, 0($s4)
	
	# i = i + 1
	addi $s2, $s2, 1
	
	j done_if

else_state:
	# ys[k] = A[j]
	li $s5, 4
	mul $s3, $s1, $s5
	add $s3, $s3, $t1
	lw $s4, 0($s3)
	
	# j = j + 1
	addi $s3, $s3, 1
	
	j done_if
	
done_if:
	
	addi $s1, $s1, 1
	blt $s1, $t3, end_loop_to_end
	
end_loop_to_end:
	
	# restore the stack
	lw $s0, 20($sp)
	lw $t3, 16($sp)
	lw $t2, 12($sp)
	lw $t1, 8($sp)
	lw $t0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 24
	jr $ra

end_split:	
	# restore stack, return from call
	lw $ra, 0($sp)
	lw $t0, 4($sp)
	lw $t1, 8($sp)
	lw $t2, 12($sp)
	lw $t3, 16($sp)
	addi $sp, $sp, 24
	jr $ra
