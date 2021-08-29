section .data
        printedStr: db '%d', 10, 0

section .text                    	; we write code in .text section
        global assFunc
        extern c_checkValidity
        extern printf

assFunc:
        push    ebp              	; save Base Pointer (bp) original value
        mov     ebp, esp         	; use Base Pointer to access stack contents (do_Str(...) activation frame)
        pushad                   	; push all signficant registers onto stack (backup registers values)        
        mov     ebx, dword [ebp+8]	; get function argument on stack
        push    ebx
        call    c_checkValidity
        
        add     esp, 4
        cmp     eax, '1'                ; check if the current char is 1
        je      num_is_even             ; if it is jump to the required actions
        
        shl     ebx, 3                  ; shift left 3 times is equal to * 8
        jmp     print                   ; jump to the printing part
        
        num_is_even:
        shl     ebx, 2                  ; shift left 2 times is equal to * 4
        
        print:
        push    ebx
        push    printedStr
        call    printf

        add     esp, 8
        popad                    	; restore all previously used registers
        mov esp, ebp			; free function activation frame
        pop ebp				; restore Base Pointer previous value (to returnt to the activation frame of main(...))
        ret				; returns from do_Str(...) function