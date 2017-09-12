// See LICENSE for license details.

#include "sifive/devices/plic.h"
#include "plic_driver.h"


void PLIC_init (interrupt_handler_t os_default_handler)
{
  int i;

  // Disable all interrupts (don't assume that these registers are reset).
  for (i = 0; i < (31 + PLIC_NUM_INTERRUPTS) / 8; i  += 4) {
	  PLIC_REG(PLIC_ENABLE_OFFSET + i) = 0;
  }

  // Set all priorities to 0 (equal priority -- don't assume that these are reset).
  for (i = 0; i < PLIC_NUM_INTERRUPTS; ++i) {
	  plic_interrupts[i] = os_default_handler;
	  /* Default priority set to 0, never interrupt */
	  PLIC_REG(PLIC_PRIORITY_OFFSET + i * 4) = 0;
  }

  // Set the threshold to 0.
  PLIC_REG(PLIC_THRESHOLD_OFFSET) = 0;
}

void PLIC_set_threshold (plic_threshold threshold){

  PLIC_REG(PLIC_THRESHOLD_OFFSET) = threshold;
}

// Each global interrupt can be enabled by setting the corresponding bit in the enables
// register. The 255 interrupt enables/disables are organized as 8 words of 32 bits.
// Bit 0 of enable word 0 represents the non-existent interrupt ID 0 and is
// hardwired to 0.

void PLIC_enable_interrupt (plic_source int_num){

	if (int_num > 0 && int_num < PLIC_NUM_INTERRUPTS) {
	  PLIC_REG(PLIC_ENABLE_OFFSET + 4 * (int_num / 32)) |= 1 << (int_num & 0x1F);
    }
}

void PLIC_disable_interrupt (plic_source int_num){

	if (int_num > 0 && int_num < PLIC_NUM_INTERRUPTS) {
      PLIC_REG(PLIC_ENABLE_OFFSET + 4 * (int_num / 32)) &= ~(1 << (int_num & 0x1F));
	}
}

// Each PLIC interrupt has a 3 bit priority stored in a 32-bit memory-mapped
// priority register. A priority value of 0 is reserved to mean “never interrupt”;
// priority 1 is the lowest active priority, and priority 7 is the highest.
void PLIC_set_priority (plic_source source, plic_priority priority){

    PLIC_REG(PLIC_PRIORITY_OFFSET + (source << PLIC_PRIORITY_SHIFT_PER_SOURCE)) = priority;
}

// Claim by an interrupt by reading the claim/complete register which returns the
// ID of the highest priority pending interrupt or zero if there is no pending
// interrupt.
plic_source PLIC_claim_interrupt(void){

  return PLIC_REG(PLIC_CLAIM_OFFSET);
}

// Signal completion of an interrupt handler by writing the interrupt ID it
// received from the claim to the claim/complete register.
void PLIC_complete_interrupt(plic_source source){

  PLIC_REG(PLIC_CLAIM_OFFSET) = source;
}


// handlers for the PLIC interrupts
interrupt_handler_t plic_interrupts[PLIC_NUM_INTERRUPTS];

void PLIC_set_handler(plic_source int_num, interrupt_handler_t handler, int priority)
{
    if (int_num > 0 && int_num < PLIC_NUM_INTERRUPTS) {
        plic_interrupts[int_num] = handler;
        PLIC_REG(PLIC_PRIORITY_OFFSET + int_num * 4) = priority;
    }
}

void external_interrupt_handler(uintptr_t mcause)
{
    int num = PLIC_REG(PLIC_CLAIM_OFFSET);
    /*
     * Interrupts have some overhead, handle all pending interrupts.
     */
    while (num) {
        /* Call interrupt handler */
        plic_interrupts[num](num);
        /* Confirm interrupt*/
        PLIC_REG(PLIC_CLAIM_OFFSET) = num;
        /* Check if other interupt is already pending */
        num = PLIC_REG(PLIC_CLAIM_OFFSET);
    }
}
