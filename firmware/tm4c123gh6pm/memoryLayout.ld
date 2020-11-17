/* SPDX-License-Identifier: BSD-3-Clause */

MEMORY
{
	FLASH	(rx):	ORIGIN = 0x00000000, LENGTH = 0x00040000
	/* This chip has 32k of RAM, so reserve 28k for the application */
	RAM		(rwx):	ORIGIN = 0x20001000, LENGTH = 0x00008000
	/* And reserve 4k for the stack (which grows down so is at the start of RAM) */
	STACK	(rwx):	ORIGIN = 0x20000FFF, LENGTH = 0x00000000
}

/*
 * Section definitions:
 *
 * .text 		- machine instructions.
 * .data 		- initialized data defined in the program.
 * .bss  		- un-initialized global and static variables (to be initialized to 0 before starting main).
 * .stack		- just contains the pointer to the stack end at the right place.
 */
SECTIONS
{
	.text :
	{
		beginText = .;
		KEEP(*(.nvic_table))
		*(.text.* .text .gnu.linkonce.t.*)
		*(.rodata.* .rodata .gnu.linkonce.r.*)

		PROVIDE(beginCtors = .);
		*(.init_array.* .ctors.*)
		KEEP(*(.init_array .ctors))
		PROVIDE(endCtors = .);
		endText = .;
	} >FLASH

	.data :
	{
		beginData = .;
		*(.data.* .data)
		*(vtable)
		endData = .;
	} >RAM AT >FLASH

	.bss :
	{
		beginBSS = .;
		*(.bss.* .bss)
		*(COMMON)
		endBSS = .;
	} >RAM

	.stack :
	{
		stackTop = .;
	} >STACK

	/* DWARF debug sections.
	 Symbols in the DWARF debugging sections are relative to the beginning
	 of the section so we begin them at 0. */
	/* DWARF 1 */
	.debug 0 : { *(.debug) }
	.line 0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo 0 : { *(.debug_srcinfo) }
	.debug_sfnames 0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges 0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end ) }
	.debug_frame 0 : { *(.debug_frame) }
	.debug_str 0 : { *(.debug_str) }
	.debug_loc 0 : { *(.debug_loc) }
	.debug_macinfo 0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames 0 : { *(.debug_varnames) }
	/* DWARF 3 */
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges 0 : { *(.debug_ranges) }
	/* DWARF Extension. */
	.debug_macro 0 : { *(.debug_macro) }
	.gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
}
