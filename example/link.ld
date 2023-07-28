ENTRY(0)

MEMORY {
    main (rwx) : ORIGIN = 0, LENGTH = 16K
}

_estack = ORIGIN(main) + LENGTH(main);

SECTIONS {
    .text : { 
        *(_start)
        *(.text)
        *(.rodata)
        *(.data)
        *(.sdata)
        *(.bss)
    } > main
}
