.data
fout:   .asciiz	"input.txt"      # filename for input
output_file:	.asciiz "output.txt"

buffer: .space 1000 		# buffer length

nextLine: .asciiz "\n" 		# Space for for printing and writing output file '\n'

comma: .asciiz "," 	# Space for for printing ','

length: .asciiz "size = " 	# Space for for writing size to output file

actv_arr: .word		# active array that holds each line in input file 
0,0,0,0,0,0,0,0,0,0

sub_arr: .word		# sub array that holds temporary sequences
0,0,0,0,0,0,0,0,0,0

fin_arr: .word		# final array holds biggest sequence at that time
0,0,0,0,0,0,0,0,0,0

temp_space: .word	# temp_space is for calculating integer in erach line at input file between commas
0,0,0

length_space: .word	# length_space is for storing each line's biggest sequence's size.
0,0

.text

main:

	li $s0, 10	# ASCII code of '\n' and max array length
	li $s1, 13
	li $s2, 0	# counter for outer loop in part3
	li $s3, 0	# counter for inner loop in part3
	li $t4, 4	# counter for inner inner loop in part3
	li $s4, 0	# sub array length
	li $s5, 0	# final array length
	li $s6, 4	# const 4
	li $t7, 0       # flag for finishing program
	
	
	
			 # Open (for reading) a file
  	li $v0, 13       # system call for open file
  	la $a0, fout     # output file name
  	li $a1, 0        # flags
  	li $a2, 0        # flags
  	syscall          # open a file (file descriptor returned in $v0)
  	
  	move $s7, $v0    # save file descriptor in $s7	
  	
  	 li $v0, 13
   	 la $a0, output_file
    	 li $a1, 1
 	 li $a2, 0
    	 syscall  # File descriptor gets returned in $v0
  	
  	move $t6, $v0    # write file descriptor in $t6
	
	j part1

part1:	   	 
											# Reads from file. It also checks end of the each line
	li $v0, 14       		# system call for read to file 
  	la $a1, buffer   		# address of buffer from which to write		
 	li $a2, 1000        		# hardcoded buffer length.
 	move $a0, $s7   		# put the file descriptor in $a0		
  	syscall          		# Read from file
  	la $t0, buffer			# Loads address of buffer to $t0. $t0 will work as string pointer	
 	j part2				# If current character is '\n' which ASCII code is 10 breaks loop
 	 					
part2:	
	
	li $a2, 40		# Set $a1 to 40. 
	li $a1, 0		# Set $a1 to 0.
	jal clr_actv_arr	# jumps to clr_actv_arr to set each element in actv_arr to 0 for each line.
	
	li $v0, 4 		
	la $a0, nextLine
	syscall			# Prints '\n' 
	li $a3, 0		# Set $a3 to 0.
	jal print_fin_arr	# jumps to print_fin_arr for each line.
	li $a3, 0		# Set $a3 to 0
	li $v0, 4 		
	la $a0, nextLine
	syscall			# Prints '\n' 
	
	beq $t7, 36, close_file		# If flag for finishing program equals to 36 goes to end of the program
	
	li $a1, 0		# Reseting a1 to 0. Because, it will works as a counter in the clr_fin_arr
	li $a3, 0
	li $s3, 0		# Sets counter for inner loop to 0
	li $a2, 4		# $a2 is upper limit for fin_arr to clear.Max array length is 10.So, making it 10 will set all elements to 0.
	mul $a2, $s1,  $a2	# i multiply it by 4.Since, in loop counter increases by 4
	jal clr_fin_arr
	
	
	
	li $a1, 0		# Reseting a1 to 0. Because, it will works as a counter in the clr_fin_arr
	li $a3, 0
	li $a2, 4		# $a2 is upper limit for fin_arr to clear.Max array length is 10.So, making it 10 will set all elements to 0.
	mul $a2, $s1,  $a2	# i multiply it by 4.Since, in loop counter increases by 4
	jal clr_fin_arr
	
	
	li $t1, 0		 	# t1 will work as a counter
	li $t2, 4		 	# t2 will work as a constant	
	la $t5, actv_arr		# Loads address of buffer to $t5
	li $s2, 0			# Resets counter for part3_outer_loop
	li $a2, 0			# Sets argument for part2_loop to 0
	li $a3, 0			# Sets argument for part2_loop to 0
	li $a1, 0			# Sets argument for part2_loop to 0
	li $t8, 0			# Sets argument for part2_loop to 0
	j part2_loop			# jumps to loop
	
part2_loop:	
	
	beq $t7, 36, part3_outer_loop				
	lbu $a0, 0($t0) 			# Loads the 1 byte of data to a0 where string pointer points.
	addi $t0, $t0, 1			# Increases string pointer by 1
	beq $a0, 0, sign_for_end		# Checks if current character is NULL, if so jumps to part3						
	beq $a0, 10, part3_outer_loop		# Checks if current character is '\n', if so jumps to part3						# Checks if current character equals to ',', if so beginning of the function
	addi $a0, $a0, -48			# Converts ASCII character to integer by substracting '0' which is 48.
	li $a2, 0
	move $s5, $a3				# $s5 is for backup of $a3 before calling load_stack. It needs current value of $a3 for end condition of loop in load_stack
	beq $a0, -4, load_stack			# if current character is ','. -4 = ',' - '0'
	beq $a0, -35, load_stack		# if current character is character before '\n'. I don't know why.But, before reading '\n' program reads ascii code 13. So, i am checking that.
							
	jal store_stack				# Before reading ',' or '\n' adds numbers to the stack	
		 
	addi $a3, $a3, 1			# Increases counter by 1
		
	j part2_loop				# Loop continues			

store_stack:
						# Stores number to the stack
	mul $a2, $a3, $s6			# Multiplies $a2 with 4. Memory address should be multiplies of 4
	sw $a0, temp_space($a2)			# Stores number to the stack at the right digit
	jr $ra					# Jumps back

load_stack:
				# Loads numbers from stack multiplies them with multiples of 10.If number in temp_space is 36.It multiplies 3 with 10 and 6 with 1.
	li $a1, 1		# result for pow
	li $a0, 1		# counter for pow
	jal pow			# Jumpts to pow function
	addi $a3, $a3, -1	# Descreases counter by 1. It starts from the highest digit and goes to the lowest digit.
	
	lw  $a0, temp_space ($a2)			# Loads the number to the a0 from stack with the correct
	
	mul $a1, $a1, $a0		# Multiplies $a1 with $a0 and stores at $a1
	add $t8, $a1, $t8 		# Adds $t8 with $a1 and stores at $t8
	
	addi $a2, $a2, 4		# Increases counter by 4
	mul $a0, $s5, $s6		# Multiplies $s5 with $s6 and stores at $a0
	blt $a2, $a0, load_stack	# If counter is smaller than $a0 loop continues
	
	sw $t8, actv_arr($t1)		# Stores $t8 to actv_arr's $t1 / 4 element
	li $a1, 0			# Resets $a1 to 0. This will be used in clr_temp_space
	li $a3, 0			# Resets $a3 to 0. This will be used in clr_temp_space
	jal clr_temp_space		# Jumps to clr_temp_space

	li $s5, 0			# Resets $s5 to 0.
	li $t8, 0			# Resets $t8 to 0.
	addi $t1, $t1, 4		# Increases actv_arr's length by 4
	j part2_loop			# Jumps to part2_loop back

pow:
					# For calculating digits. This function is for calculating multi digit numbers
	beq $a3, 1, finish_pow		# if number has 1 digit 
	
	addi $a0, $a0, 1		# increases digit by 1
	mul $a1, $a1, $s0		# Multiplies the number with multiplies of 10. 10's powers.
	bgt  $a3, $a0, pow		# loop continu?es until it reaches end
	jr $ra
	
finish_pow:		# For exiting pow function
	
	jr $ra
	
clr_temp_space:
					# Sets every element in temp_space to 0
	sw $zero, temp_space($a1)	# Stores 0 at temp_space's $a1's location
	addi $a1, $a1, 4		# Increases counter by 4
	blt $a1, 12, clr_temp_space	# Iterates until counter reaches third digit
	jr $ra	

sign_for_end:
				# $t7 is flag for ending program.
	li $t7, 36		# Sets flag true
	j load_stack		# Jumps to the load_stack	
									
part3_outer_loop:
																																																															
  	li $a1, 0		# First index of sub array needs to become 0
	li $a2, 40		# Last index of sub array needs to become 0
	jal clr_sub_arr		# Sets every element on sub array 0 at every iteration on outer loop
	
	lw $a3, actv_arr($s2)	# Loads actv_arr[i] to $a3
	sw $a3, sub_arr($s4)	# Adds value at the $a3 to the end of the sub_arr
	addi $s4, $s4, 4
		
	li $a3, 0		# Resets counter for writing file			
	bgt $s2, $t1, write_to_file		# If counter is bigger than actv_arr's length jump to write_to_file
	
	move $s3, $s2			# Sets $s3 to outer loop's counter
	addi $s2, $s2, 4		# Increases outer loop's counter by 4
	j part3_inner_loop		# Jumps to inner loop
																	
			
part3_inner_loop:
				
	li $a1, 0		# First index of sub array needs to become 0
	li $a2, 40		# Last index of sub array needs to become 0
	jal clr_sub_arr					# Sets element of sub_arr to 0 at every iteration of loop
	
	move $a3, $s2		# Sets $a3 to outer loops counter
	addi $a3, $a3, -4	# Decreases $a3 by 4
	lw $a3, actv_arr($a3)	# Loads actv_arr[i] to $a3
	sw $a3, sub_arr($s4)	# Adds value at the $a3 to the end of the sub_arr
	addi $s4, $s4, 4	# Increases length of sub_arr by 1(4 byte)
		
	addi $a2, $s4, -4	# length - 1
	lw $a3 actv_arr($a1)	# Loads actv_arr's $a1 / 4 th element to $a3
	lw $a0 sub_arr($a2)	# Loads sub_arr's $a2 / 4 th element to $a0
	
	lw $a2, actv_arr($s3)				# Loads actv_arr[j]
	lw $t8, sub_arr + 0				# Loads sub_arr[0]
	addi $s3, $s3, 4				# Increases counter by 4		
	blt $a2, $t8, part3_inner_loop			# if sub_arr[0] > actv_arr[j] passes rest of the loop and jumps to beginning of the function
	
	move $t4, $s3					# Resets counter for part3_inner_inner_loop
	jal part3_inner_inner_loop			# jumps to the part3_inner_inner_loop
	
	li $a3, 0					# Sets $a3 to 0. Because, $a3 will be used in print_sub_arr
	jal print_sub_arr				# Prints sub_arr
	li $a3, 0
								
	li $a2, 0					# Resets counter for add_fin_arr to 0	
	bgt $s4, $s5, add_fin_arr			# if sub_arr_len > fin_arr_len jumps to	add_fin_arr	
			
	blt $s3 , $t1, part3_inner_loop			# if counter reached end of the array
	j part3_outer_loop	
	
part3_inner_inner_loop:
							# This function is for searchin a bigger number than sub_arr's last element
	lw $a2, actv_arr($t4)				# Loads actv_arr[k]
	move $a1, $s4					# Sets $a1 to sub_arr's length
	addi $a1, $a1 , -4				# Decreases $a1 by 4
	lw $t9, sub_arr($a1)				# Loads sub_arr[sub_arr_len - 1]			
	
	lw $a3, actv_arr($t4)				# Loads actv_arr[k] to $a3 in order to add it to sub_arr 
	addi $t4, $t4, 4				# Increases counter by 4
	bgt $a2, $t9, add_sub_arr			# If a bigger number than sub_arr[sub_arr_len - 1] is found jumps add_sub_arr
	blt $t4, $t1, part3_inner_inner_loop		# if counter reached enf of the array
	jr $ra						# jumps back to part3_inner_loop
			
add_sub_arr:
	
	sw $a3, sub_arr($s4)		# Adds value at the $a3 to the end of the sub_arr
	addi $s4, $s4, 4		# Increases the sub_arr length by 4
	j part3_inner_inner_loop	# Jumps to part3_inner_inner_loop

add_fin_arr:	
					# Adds every element in sub array to final array
	lw $a1, sub_arr($a2)		# Loads sub_arr's $a2 / 4 'th element to $a1 ($a2 / 4. Because memory addresses are multiplicities of 4)
	sw $a1, fin_arr($a2)		# Stores the recently loadedd number to fin_arr's $a2'th location
	addi $a2, $a2, 4		# Incereases counter by 4	
	blt $a2, $s4 add_fin_arr	# Loop continues until counter reaches sub_arr's length * 4
	move $s5, $s4			# Sets new fin_arr length		
	j part3_inner_loop 		# Jump to part3_inner_loop after loop ends
			
clr_sub_arr:
					# Sets every element on sub_arr to 0
	li $s4, 0			# Sets length of sub_arr to 0
	sw $zero, sub_arr($a1)		# Sets sub_arr's $a1 / 4 'th element to 0
	addi $a1, $a1, 4		# Incereases counter by 4
	blt $a1, $a2, clr_sub_arr	# Loop continues until counter reaches actv_arr's length * 4		
	jr $ra				# Jumps back
	
clr_fin_arr:
					# Sets every element on fin_arr to 0
	li $s5, 0			# Sets length of fin_arr to 0	
	sw $zero, fin_arr($a1)		# Sets fin_arr's $a1 / 4 'th element to 0
	addi $a1, $a1, 4		# Incereases counter by 4
	blt $a1, $a2, clr_fin_arr	# Loop continues until counter reaches actv_arr's length * 4		
	jr $ra				# Jumps back
		
	
clr_actv_arr:
					# Sets every element on actv_arr to 0
	sw $zero, actv_arr($a1)		# Sets actv_arr's $a1 / 4 'th element to 0
	addi $a1, $a1, 4		# Incereases counter by 4
	blt $a1, $a2, clr_actv_arr	# Loop continues until counter reaches actv_arr's length * 4		
	jr $ra				# Jumps back
							
write_to_file:
		
	beq $a3, $zero, write_to_file_loop
	
	move $a0, $t6		# Syscall 15 requieres file descriptor in $t6
	li $v0, 15		
	la $a1, length		# Loads address of the string "size = "
	li $a2, 7		# Size of length is 7. So 7 bytes will be enough
	syscall
	
	move $t3, $s5			# $t3 is temporary space for seting fin_arr' length
	div $t3, $t3, $s6		# Divides $t3 to 4 for calculating fin_arr's length
	addi $t3, $t3, 48		# Adding 48 to fin_arr's length to calculating it's ascii code
	sw $t3, length_space + 0	# Stores ascii code of fin_arr's length in order to write to output file
	
	move $a0, $t6			# Syscall 15 requieres file descriptor in $t6
	li $v0, 15			
	la $a1, length_space + 0	# loads address ascii code of fin_arr's length
	li $a2, 1			# 1 byte is enough space for fin_arr's length	
	syscall
		
	move $a0, $t6		# Syscall 15 requieres file descriptor in $t6
	li $v0, 15
	la $a1, nextLine	#writes '\n' to file
	li $a2, 1		# 1 byte is enough space for '\n' character
	syscall

	j part2
	
write_to_file_loop:

	move $a0, $t6  		# Syscall 15 requieres file descriptor in $t6
    	li $v0, 15
    	lw $a1, fin_arr($a3)	# Loading the number from stack in order to convert it to ascii format
    	addi $a1, $a1, 48 	#Converts to ascii format
    	sw $a1, fin_arr($a3) 	# Storing number's ascii format to stack in order to write it to the file
    	
	la $a1, fin_arr($a3)	# Loads $a3'th element of fin_arr
	addi $a3, $a3, 4	# Increases counter by 4
	li $a2, 1		# 1 byte is enough space for character of an integer
	syscall			# Writes to file
	
	addi $a3, $a3, -4	# Decreasing $a3 by 4 in order to store values at the correct position
	lw $a1, fin_arr($a3)	# Loading the number from stack in order to convert it back to int
    	addi $a1, $a1, -48 	# Converts ascii format to int
    	sw $a1, fin_arr($a3) 	# Storing number back to stack with it's int value
    	addi $a3, $a3, +4	# Increasing $a3 back for continueing loop
    	
    	move $a0, $t6		# Syscall 15 requieres file descriptor in $t6
	li $v0, 15		
	la $a1, comma		# Loads comma's address
	li $a2, 1	
	syscall			# Writes comma to file
		
	blt $a3, $s5, write_to_file_loop	# Loop continues until counter reaches fin_arr's length
	j write_to_file																																																				
																																																				
close_file:
				# Close the file 
  	li $v0, 16       	# system call for close file
  	move $a0, $s7    	# Restore fd
  	syscall          	# close file
  	
  	move $a0, $t6		# Syscall 16 requieres file descriptor in $t6
  	li $v0, 16  		# $a0 already has the file descriptor
    	syscall
  					
	li $v0, 10		# Exits the program
	move $a0, $t6
	syscall
	
print_fin_arr:
			#Prints fin_arr
	li $v0, 1
	lw $a0, fin_arr($a3)
	syscall
	li $v0, 4
	la $a0, comma
	syscall
	addi $a3, $a3, 4
	blt $a3, $s5, print_fin_arr
	li $v0, 4
	la $a0, nextLine
	syscall
	jr $ra  
	
print_sub_arr:
			#Prints sub_arr
	li $v0, 1
	lw $a0, sub_arr($a3)
	syscall
	li $v0, 4
	la $a0, comma
	syscall
	addi $a3, $a3, 4
	blt $a3, $s4, print_sub_arr
	li $v0, 4
	la $a0, nextLine
	syscall
	jr $ra  			
