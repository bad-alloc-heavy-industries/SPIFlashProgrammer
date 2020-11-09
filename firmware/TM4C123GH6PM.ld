/* SPDX-License-Identifier: BSD-3-Clause */

MEMORY
{
	FLASH	(rx):	ORIGIN = 0x00000000, LENGTH = 0x00040000
	RAM		(rwx):	ORIGIN = 0x20000000, LENGTH = 0x00008000
	STACK	(rwx):	ORIGIN = 0x20007FFF, LENGTH = 0x00000000
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
		_start_text = .;
		KEEP(*(.nvic_table))
		*(.text.*)
		*(.rodata.*)
		_end_text = .;
	} >FLASH

	.data :
	{
		_start_data = .;
		*(.data.*)
		*(vtable)
		_end_data = .;
	} >RAM AT >FLASH

	.bss :
	{
		_start_bss = .;
		*(.bss.*)
		*(COMMON)
		_end_bss = .;
	} >RAM

	.stack :
	{
		_stack_top = .;
	} >STACK
}
