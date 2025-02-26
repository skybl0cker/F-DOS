OUTPUT_FORMAT(binary)
SECTIONS {
    . = 0x10000;            /* Physical address 0x1000:0x0000 = 0x10000 */
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}