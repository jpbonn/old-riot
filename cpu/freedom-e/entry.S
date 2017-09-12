/*
   This file is just a copy of freedom-e-sdk/bsp/env/entry.S from the
   SiFive Freedom SDK for the HiFive board with minor changes.
*/

/* See LICENSE for license details */

#ifndef ENTRY_S
#define ENTRY_S

#include "encoding.h"
#include "sifive/bits.h"
#include "context_frame.h"


/* from platform.h TODO:fix this hard code.... */
CLINT_CTRL_ADDR = 0x02000000

  .section      .text.entry
  .align 2
  .global trap_entry
trap_entry:
  addi sp, sp, -CONTEXT_FRAME_SIZE

  sw s0, s0_OFFSET(sp)
  sw s1, s1_OFFSET(sp)
  sw s2, s2_OFFSET(sp)
  sw s3, s3_OFFSET(sp)
  sw s4, s4_OFFSET(sp)
  sw s5, s5_OFFSET(sp)
  sw s6, s6_OFFSET(sp)
  sw s7, s7_OFFSET(sp)
  sw s8, s8_OFFSET(sp)
  sw s9, s9_OFFSET(sp)
  sw s10, s10_OFFSET(sp)
  sw s11, s11_OFFSET(sp)
  sw ra, ra_OFFSET(sp)
  sw gp, gp_OFFSET(sp)
  sw tp, tp_OFFSET(sp)
  sw t0, t0_OFFSET(sp)
  sw t1, t1_OFFSET(sp)
  sw t2, t2_OFFSET(sp)
  sw t3, t3_OFFSET(sp)
  sw t4, t4_OFFSET(sp)
  sw t5, t5_OFFSET(sp)
  sw t6, t6_OFFSET(sp)
  sw a0, a0_OFFSET(sp)
  sw a1, a1_OFFSET(sp)
  sw a2, a2_OFFSET(sp)
  sw a3, a3_OFFSET(sp)
  sw a4, a4_OFFSET(sp)
  sw a5, a5_OFFSET(sp)
  sw a6, a6_OFFSET(sp)
  sw a7, a7_OFFSET(sp)

  csrr a0, mcause
  /* is the high bit set in mcause? => interrupt */
  bltz a0, handle_interrupt
  /* otherwise something else caused us to trap we don't know how to handle it */
1:
  j 1b


handle_interrupt:
  /* is it a software interrupt? */
  li t0, 0x80000003
  beq a0, t0, context_switch

  /* otherwise something else caused us to trap we don't know how to handle it */
2:
  j 2b


trap_exit:
  # Remain in M-mode after mret
  li t0, MSTATUS_MPP
  csrs mstatus, t0

  lw s0, s0_OFFSET(sp)
  lw s1, s1_OFFSET(sp)
  lw s2, s2_OFFSET(sp)
  lw s3, s3_OFFSET(sp)
  lw s4, s4_OFFSET(sp)
  lw s5, s5_OFFSET(sp)
  lw s6, s6_OFFSET(sp)
  lw s7, s7_OFFSET(sp)
  lw s8, s8_OFFSET(sp)
  lw s9, s9_OFFSET(sp)
  lw s10, s10_OFFSET(sp)
  lw s11, s11_OFFSET(sp)
  lw ra, ra_OFFSET(sp)
  lw gp, gp_OFFSET(sp)
  lw tp, tp_OFFSET(sp)
  lw t0, t0_OFFSET(sp)
  lw t1, t1_OFFSET(sp)
  lw t2, t2_OFFSET(sp)
  lw t3, t3_OFFSET(sp)
  lw t4, t4_OFFSET(sp)
  lw t5, t5_OFFSET(sp)
  lw t6, t6_OFFSET(sp)
  lw a0, a0_OFFSET(sp)
  lw a1, a1_OFFSET(sp)
  lw a2, a2_OFFSET(sp)
  lw a3, a3_OFFSET(sp)
  lw a4, a4_OFFSET(sp)
  lw a5, a5_OFFSET(sp)
  lw a6, a6_OFFSET(sp)
  lw a7, a7_OFFSET(sp)

  addi sp, sp, CONTEXT_FRAME_SIZE
  mret

 context_switch:
   /* clear the software interrupt */
  li t0, CLINT_CTRL_ADDR
  sw zero, (t0)

  /* save the current thread's PC on the stack */
  csrr a0, mepc
  sw a0, pc_OFFSET(sp)

  /* get the active thread - it may be 0 if none currently active */
  lw t0, sched_active_thread
  /* was there a previously running thread? */
  beqz t0, no_sp_save
  /* if so, save the thread's SP in the _thread structure */
  sw sp,SP_OFFSET_IN_THREAD(t0)
no_sp_save:


  /* all current thread state is saved - schedule a new thread */
  call sched_run
  lw tp, sched_active_thread

  /* set the SP from the newly scheduled thread */
  lw sp, SP_OFFSET_IN_THREAD(tp)

  /* restore the PC */
  lw a0, pc_OFFSET(sp)
  csrw mepc, a0

  j trap_exit


#endif
