; This file is just for testing!

[GLOBAL print]

print:
    push rbp
	mov rbp, rsp
	mov QWORD [rbp-8], rdi
    mov rax, QWORD [rbp-8]
    call _print
	nop
	pop rbp
	ret

_print:
    push rax
    mov rbx, 0
_printLoop:
    inc rax
    inc rbx
    mov cl, [rax]
    cmp cl, 0
    jne _printLoop
    mov rax, 1
    mov rdi, 1
    pop rsi
    mov rdx, rbx
    syscall
    ret
