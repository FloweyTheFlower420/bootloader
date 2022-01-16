OUTPUT_FORMAT("binary")
phys = 0x8000;

SECTIONS
{
    . = 0x7e00;

    .text :
    {
        code = .;
	*(.text)
    }

    .rodata BLOCK(1) : ALIGN(1)
    {
    	*(.rodata)
    }

    .data BLOCK(1) : ALIGN(1)
    {
    	data = .;
    	*(.data)
    }

    .bss BLOCK(1) : ALIGN(1)
    {
    	bss = .;
    	*(COMMON)
    	*(.bss)
    	*(.stack)
    }

    end = .;

    /DISCARD/ :
    {
    	*(.comment)
    	*(.eh_frame)
    	*(.note.gnu.build-id)
    }
}
