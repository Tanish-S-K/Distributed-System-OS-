[bits 32]
[extern kernel_main]

global _start
_start:
    cli
    call kernel_main
.hang:
    hlt
    jmp .hang
