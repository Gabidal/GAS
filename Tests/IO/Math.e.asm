.intel_syntax noprefix
.global main
.global Start_Test
.section .text
sum:
add ecx, 3
mov eax, ecx
ret 
ret 


Start_Test:
push rbx
mov ebx, 3
mov ecx, ebx
call sum
mov ecx, eax
mov r8d, ecx
mov r9, rdx
mov eax, r8d
mul ebx
mov r8d, eax
mov rdx, r9
mov r8d, r8d
mov r9d, r8d
xor edx, edx
mov eax, r9d
div ecx
mov r9d, eax
mov r9d, r9d
add r9d, ebx
add r9d, ecx
sub r9d, r8d
mov eax, r9d
pop rbx
ret 
mov ecx, 1067030938
movd xmm0, ecx
movss xmm0, xmm0
mov ecx, 1075000115
movd xmm1, ecx
movss xmm1, xmm1
call Test_Fpu
cvttss2si ecx, xmm0
mov eax, ecx
pop rbx
ret 
mov eax, -6
pop rbx
ret 
pop rbx
ret 


Test_Fpu:
mov ecx, -1110651699
movd xmm0, ecx
movss xmm0, xmm0
ret 
ret 


main:
call Start_Test
mov eax, 1
ret 
ret 


char:
mov rax, rcx
ret 
ret 


.section .data
