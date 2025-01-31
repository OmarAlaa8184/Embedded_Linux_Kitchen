.global _start
.section .text
.align 4

_start:
    // Set stack pointer (adjust address if needed)
    ldr x0, =stack_top
    mov sp, x0

    // Call main()
    bl main

    // Halt if main() returns
hang:
    wfi
    b hang

.section .bss
.align 4
stack:
    .skip 4096  // Reserve 4KB stack
stack_top:

