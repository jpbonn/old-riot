

#include <encoding.h>
#include "arch/irq_arch.h"

/* Return current interrupt state and enable interrupts */
unsigned int irq_arch_enable(void)
{
    return set_csr(mstatus, MSTATUS_MIE) & MSTATUS_MIE;
}

/* Return current interrupt state and disable interrupts */
unsigned int irq_arch_disable(void)
{
	 return clear_csr(mstatus, MSTATUS_MIE) & MSTATUS_MIE;
}


void irq_arch_restore(unsigned int state)
{
    if (state & MSTATUS_MIE) {
    	set_csr(mstatus, MSTATUS_MIE);
    }
    else {
    	clear_csr(mstatus, MSTATUS_MIE);
    }
}

int irq_arch_in(void)
{
	return !(read_csr(mstatus) & MSTATUS_MIE);
}
