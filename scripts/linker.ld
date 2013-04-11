ENTRY (loader)

SECTIONS
{
    . = 0x00100000;

    .text ALIGN (0x1000) :
    {
        code = .; _code = .; __code = .;
        *(.text)
    }

    .data ALIGN (0x1000) :
    {
        data = .; _data = .; __data = .;
        *(.data)
        *(.rodata)
    }

    .bss ALIGN (0x1000) :
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
    }

    end = .; _end = .; __end = .;
}
