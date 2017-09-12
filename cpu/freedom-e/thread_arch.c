

/**
 * @ingroup     cpu_freedom-e
 * @{
 *
 * @file
 * @brief       Implementation of the kernel's architecture dependent thread interface
 *
 *
 * @}
 */

#include <stdio.h>
#include <assert.h>

#include "arch/thread_arch.h"
#include "thread.h"
#include "sched.h"
#include "irq.h"
#include "cpu.h"
#include "board.h"
#include "platform.h"
#include "panic.h"
#include "context_frame.h"
#include "encoding.h"



/**
 * @brief   Noticeable marker marking the beginning of a stack segment
 */
#define STACK_MARKER                (0x77777777)

/**
 * @brief Initialize a thread's stack
 *
 * RIOT saves the tasks registers on the stack, not in the task control
 * block.  thread_arch_stack_init() is responsible for allocating space for
 * the registers on the stack and adjusting the stack pointer to account for
 * the saved registers.
 *
 * The stack_start parameter is the bottom of the stack (low address).  The
 * return value is the top of stack: stack_start + stack_size - space reserved
 * for thread context save - space reserved to align stack.
 *
 * thread_arch_stack_init is called for each thread.
 *
 * RISCV ABI is here: https://github.com/riscv/riscv-elf-psabi-doc
 * From ABI:
 * The stack grows downwards and the stack pointer shall be aligned to a
 * 128-bit boundary upon procedure entry, except for the RV32E ABI, where it
 * need only be aligned to 32 bits. In the standard ABI, the stack pointer
 * must remain aligned throughout procedure execution. Non-standard ABI code
 * must realign the stack pointer prior to invoking standard ABI procedures.
 * The operating system must realign the stack pointer prior to invoking a
 * signal handler; hence, POSIX signal handlers need not realign the stack
 * pointer. In systems that service interrupts using the interruptee's stack,
 * the interrupt service routine must realign the stack pointer if linked
 * with any code that uses a non-standard stack-alignment discipline, but
 * need not realign the stack pointer if all code adheres to the standard ABI.
 *
 * (FYI- thread.c "allocate(s) our thread control block at the top of our
 * stackspace" which makes it easy to find if needed.)
 *
 * @param[in] task_func     pointer to the thread's code
 * @param[in] arg           argument to task_func
 * @param[in] stack_start   pointer to the start address of the thread
 * @param[in] stack_size    the maximum size of the stack
 *
 * @return                  pointer to the new top of the stack (128bit aligned)
 *
 *
 * TODO: does this need a full stack frame for the return to sched_task_exit()?
 */
char *thread_arch_stack_init(thread_task_func_t task_func, void *arg, void *stack_start, int stack_size)
{
	struct context_switch_frame *sf;
	uint32_t *reg;
	uint32_t *stk_top;

	/* calculate the top of the stack */
    stk_top = (uint32_t *)((uintptr_t)stack_start + stack_size);

    /* Put a marker at the top of the stack.  This is used by
     * thread_arch_stack_print to determine where to stop dumping the
     * stack.  Note the stack frame structure is 31 entries so the
     * marker is unlikely use additional space (change the adjusted
     * stack alignment).
     */
    stk_top--;
    *stk_top = STACK_MARKER;

	/* per ABI align stack pointer to 16 byte boundary. */
    stk_top = (uint32_t *)(((uint32_t)stk_top) & ~((uint32_t)0xf));

	/* reserve space for the stack frame. */
    stk_top = (uint32_t *)((uint8_t *) stk_top - sizeof(*sf));

	/* populate the stack frame with default values for starting the thread. */
    sf = (struct context_switch_frame *) stk_top;
    reg = &sf->a7;

    while (reg != &sf->pc) {
    	*reg-- = 0;
    }
    sf->pc = (uint32_t) task_func;
    sf->a0 = (uint32_t) arg;

    /* if the thread exits go to sched_task_exit() */
    sf->ra = (uint32_t) sched_task_exit;

	return (char *) stk_top;
}



/**
 * @brief   Get the number of bytes used on the ISR stack
 */
int thread_arch_isr_stack_usage(void)
{
    return 1;
    //TODO: FIXME

}

/**
 * @brief   Get the current ISR stack pointer
 */
void *thread_arch_isr_stack_pointer(void)
{
	 return (char *)0;
	 //TODO: FIXME
}

/**
 * @brief   Get the start of the ISR stack
 */
void *thread_arch_isr_stack_start(void)
{
	 return (char *)0;
	 //TODO: FIXME
}

/**
 * @brief Print the current stack to stdout
 */
void thread_arch_stack_print(void)
{
    int count = 0;
    uint32_t *sp = (uint32_t *)sched_active_thread->sp;

    printf("printing the current stack of thread %" PRIkernel_pid "\n",
           thread_getpid());
    printf("  address:      data:\n");

    do {
        printf("  0x%08x:   0x%08x\n", (unsigned int)sp, (unsigned int)*sp);
        sp++;
        count++;
    } while (*sp != STACK_MARKER);

    printf("current stack size: %i byte\n", count);
}


/**
 * @brief Get the offset of "member" in the context_switch_frame structure
 * for assembly instructions.
 */
#define CONTEXT_OFFSET(member)     \
    [member ## _offset] "i" (offsetof(struct context_switch_frame, member))

/**
 * @brief Start or resume threading by loading a threads initial information
 * from the stack
 *
 * sched_active_thread is not valid when thread_arch_start_threading() is
 * called.  sched_run() is called to determine the next valid thread.
 *
 * This is called is two situations: 1) after the initial main and idle threads
 * have been created and 2) when a thread exits.
 *
 * It will not be called from an ISR in either case but may be called to resume
 * a thread that has already started. In either case the current context is
 * being discarded so we're free to step on things. For a resumed thread the
 * full state needs to be restored.  On initial start up only 4 regs need to be
 * initialized on RISC V. For initial startup we can just jump to an
 * address stored in an unused register to startup "jr Rx".  In the case
 * of resume we cannot load the PC directly and all registers must be preserved
 * so we must do an interrupt and full context switch.
 */
void NORETURN thread_arch_start_threading(void)
{
	/* enable interrupts */
	irq_arch_enable();

	/* start the thread */
	thread_arch_yield();

	assert("Should not be here!" && 0);
	UNREACHABLE();
}


/**
 * @brief Pause the current thread and schedule the next pending, if available
 */
void thread_arch_yield(void)
{
	/* set the software interrupt bit */
	CLINT_REG(CLINT_MSIP) = 1;
}
