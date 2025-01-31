ENTRY(_start)

SECTIONS {
    . = 0x80000;  /* ARM starts execution here */

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}

