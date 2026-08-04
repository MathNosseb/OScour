ENTRY(_start)

SECTIONS {
    . = 0x1000;
    .text :
	{
		KEEP(*(.text.entry))
		*(.text)
	}
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
