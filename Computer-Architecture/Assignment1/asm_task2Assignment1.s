section	.rodata					; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .data
	return: db 0				; we define the return value here to be 0 by default

section .bss					; we define (global) uninitialized variables in .bss section
	an: resb 33					; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]

section .text
	global convertor
	extern printf
	extern main

convertor:
	push 	ebp
	mov 	ebp, esp	
	pushad			

	mov 	ecx, dword [ebp+8]	; get function argument (pointer to string)
	mov		ebx, an				; ebx will be the register pointing to an 
	mov		eax, 0				; eax is used to initialize an

	cmp		byte[ecx], 'q'		; if the input is 'q', immediately return with return value 1 
	jne		init_an				; otherwise we initiate an
	inc		byte[return]
	jmp		code_end

	init_an:					; using eax, loop over an and set all values to 0
	mov		byte[an + eax], 0
	inc		eax
	cmp		byte[an + eax], 0
	jne		init_an

	char_to_hex:
	mov		dl, byte[ecx]		; move the current char into the 8bit register dl
	cmp		dl, 'A'				; compare with the char value of 'A'
	jge		letter				; this means dl contains a letter, act accordingly
	sub		dl, '0'				; otherwise it's a number, sub '0' from it to get the actual value
	jmp		to_binary
	letter:
	sub		dl, 'A'				; if it's a letter, sub '0' from it and add 10 to get the actual value
	add		dl, 10	
	
	to_binary:
	shl		dl, 4				; shift left 4 times to get the number to the end of dl
	shl		dl, 1 				; first shift left to remove the msb
	setc 	byte[ebx]			; the msb is now on the carry flag
	add		byte[ebx], '0'		; add the char '0' to the value of the carry flag
	inc		ebx
	shl		dl, 1 				;second shift left
	setc 	byte[ebx]
	add		byte[ebx], '0'
	inc		ebx
	shl		dl, 1 				;third shift left
	setc 	byte[ebx]
	add		byte[ebx], '0'
	inc		ebx
	shl		dl, 1 				;fourth shift left
	setc 	byte[ebx]
	add		byte[ebx], '0'
	inc		ebx

	next_char:
	inc 	ecx      	    	; increment ecx value; now ecx points to the next character of the string
	cmp 	byte [ecx], 0 | 10 	; check if the next character (character = byte) is zero (i.e. null string termination)
	jnz 	char_to_hex 

	push 	an					; call printf with 2 arguments -  
	push 	format_string		; pointer to str and pointer to format string
	call 	printf
	add 	esp, 8				; clean up stack after call
	
	code_end:
	popad
	mov		eax,[return]
	mov 	esp, ebp	
	pop 	ebp
	ret
