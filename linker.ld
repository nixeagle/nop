ENTRY (loader)

SECTIONS
{
	. = 0x100000;
        kernel_start = .;
    .__mbHeader : {
         *(.__mbHeader)
    }
	.text ALIGN(4096) :
	{
		*(.text*)
		*(.gnu.linkonce.t*)
	}

	.rodata ALIGN(4096) :
	{
		*(.rodata*)
		*(.gnu.linkonce.r*)
	}

	.data ALIGN(4096) :
	{
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;

		start_dtors = .;
		*(.dtor*)
		end_dtors = .;

		*(.data*)
		*(.gnu.linkonce.d*)
	}

	.bss ALIGN(4096) :
	{
		*(.COMMON*)
		*(.bss*)
		*(.gnu.linkonce.b*)
	}

	 /DISCARD/ :
	 {
		*(.comment)
		*(.eh_frame) /* You should discard this unless you're implementing runtime support for C++ exceptions. */
	 }

         kernel_end = .;
}

/* SECTIONS{ */
/*     . = 0x00100000; */

/*     .__mbHeader : { */
/*          *(.__mbHeader) */
/*     } */


/*     .text :{ */
/*         *(.text) */
/*     } */

/*     .rodata ALIGN (0x1000) : { */
/*         *(.rodata) */
/*     } */

/*     .data ALIGN (0x1000) : { */
/*         *(.data) */
/*     } */

/*     .bss : { */
/*         sbss = .; */
/*         *(COMMON) */
/*         *(.bss) */
/*         ebss = .; */
/*     } */
/* } */
