mov eax, 2
mov ebx, [ebp - 8]
mov ebx, [ebx + 4]
mov ebx, eax

main: 
push ebp
mov ebp, esp
mov ecx, 3
mov edx, [ebp - 4]
mov edx, ecx

mov esp, ebp
pop ebp
ret

