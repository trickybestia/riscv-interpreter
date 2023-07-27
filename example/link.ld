MEMORY {
    main (rwx) : ORIGIN = 0, LENGTH = 16K
}

SECTIONS {
    .text : { 
        *(.entry)
        *(.text)
        *(.rodata)
        *(.data)
        *(.sdata)
        *(.bss)
    } > main
}
