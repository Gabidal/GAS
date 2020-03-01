global main
main: 
call _start
mov AL , byte 0

ret 
_start: 
; The Right Side in = has already initialized.
mov [EBP  - 4], byte 2
_while4: 
mov EAX , dword [EBP  - 4]

cmp EAX , byte 3
jnl _while4END
mov EBX , dword [EBP  - 4]

add EBX , byte 1
; The Left Side in + has already initialized.
add EBX , dword [EBP  - 4]
; The Right Side in = has already initialized.
mov [EBP  - 4], EBX 
jmp _while4
_while4END: 
mov AL , AL 

ret 
