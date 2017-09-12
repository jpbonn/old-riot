// See LICENSE file for licence details

#ifndef PLIC_DRIVER_H
#define PLIC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

typedef uint32_t plic_source;
typedef uint32_t plic_priority;
typedef uint32_t plic_threshold;

typedef void (* interrupt_handler_t)(plic_source num);


void PLIC_init (interrupt_handler_t default_handler);

void PLIC_set_threshold (plic_threshold threshold);

void PLIC_enable_interrupt (plic_source source);

void PLIC_disable_interrupt (plic_source source);

void PLIC_set_priority (plic_source source,	plic_priority priority);

plic_source PLIC_claim_interrupt(void);

void PLIC_complete_interrupt(plic_source source);

// from Apache MyNewt
// https://github.com/apache/mynewt-core/pull/396/files

/*
 * External interrupt table
*/
extern interrupt_handler_t plic_interrupts[];

/*
 * Set handler for external interrupt
 */
void PLIC_set_handler(plic_source int_num, interrupt_handler_t handler, int priority);

#ifdef __cplusplus
}
#endif

#endif  // PLIC_DRIVER_H
