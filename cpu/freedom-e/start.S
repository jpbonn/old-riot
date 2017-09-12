/*
   This file is just a copy of freedom-e-sdk/bsp/env/start.S from the
   SiFive Freedom SDK for the HiFive board with minor changes.

   Code for SMP processors has been removed.
*/

// See LICENSE for license details.

/* This is defined in sifive/platform.h, but that can't be included from
 * assembly. */
#define CLINT_CTRL_ADDR 0x02000000

    .section        .rodata
hello_str:
    .ascii  "\n\n***  Board Initialized!  ***\n"
    .set hello_size, .-hello_str

	.section .init
	.globl _start
	.type _start,@function

_start:
	.cfi_startproc
	.cfi_undefined ra
.option push
.option norelax
	la gp, __global_pointer$
.option pop
	la sp, _sp

	/* Load data section */
	la a0, _data_lma
	la a1, _data
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:

	/* Clear bss section */
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:


#ifndef __riscv_float_abi_soft
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrs mstatus, t0
	csrr t1, mstatus
	and t1, t1, t0
	beqz t1, 1f
	fssr x0
1:
#endif

    /* Setup up a stack frame for the debugger?? */
	auipc ra, 0
	addi sp, sp, -16
#if __riscv_xlen == 32
	sw ra, 8(sp)
#else
	sd ra, 8(sp)
#endif

	call board_init

	/* Call global constructors */
	la a0, __libc_fini_array
	call atexit
	/* initialize UART and other .init section functions */
	call __libc_init_array

    /* TODO: remove this debug message */
	li a0,0
    la a1, hello_str
    li a2, hello_size
	call uart_write

    /* Startup the kernel:
     * Creates main and idle threads and starts them */
    call kernel_init
1:
	j 1b

	.cfi_endproc
