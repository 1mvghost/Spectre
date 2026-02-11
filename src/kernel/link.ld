OUTPUT_FORMAT(elf64-x86-64)
ENTRY(main)

PHDRS {
    limine_requests PT_LOAD;
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
}
SECTIONS {
    . = 0xffffffff80000000;

    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :limine_requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    _text = .;
    .text : {
        *(.text .text.*)
    } :text
    
    . = ALIGN(CONSTANT(MAXPAGESIZE));
    _rodata = .;
    .rodata : {
        *(.rodata .rodata.*)
    } :rodata
    
    . = ALIGN(CONSTANT(MAXPAGESIZE));
    _data = .;
    .data : {
        *(.data .data.*)
    } :data
    
    . = ALIGN(CONSTANT(MAXPAGESIZE));
    _bss = .;
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    end = .;

    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
    }
}