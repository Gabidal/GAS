main: 
push ebp
mov ebp, esp

mov dword [ebp - 4], 2
mov dword [ebp - 8], 3
if0: 
mov eax, [ebp - 4]
sub eax, [ebp - 8]
cmp eax, [ebp - 4]
jg if1
sub eax, 1
mov [ebp - 4], eax

if0END: 
jmp else2
if1: 
cmp eax, [ebp - 8]
jne else2
mov ebx, [ebp - 8]
sub ebx, 1
mov [ebp - 8], ebx

if1END: 
jmp else2
else2: 
mov [ebp - 4], ebx
else2END: 
if3: 
cmp ebx, [ebp - 8]
jne if3END
mov ecx, [ebp - 8]
add ecx, ebx
mov [ebp - 4], ecx

if3END: 
mov esp, ebp
pop ebp
ret

