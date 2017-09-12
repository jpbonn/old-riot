

/**
 * @ingroup     cpu_freedom-e
 * @ingroup     drivers_periph_pm
 * @{
 *
 * @file
 * @brief        common periph/pm functions
 *
 * @}
 */

#include "periph/pm.h"
#include "platform.h"
#include "encoding.h"

void pm_off(void)
{
   /* No Generic Power off Mechanism TODO: see what we can do with the AON hardware. */
	while (1) {
		// wfi prevents the debugger from connecting.
		//  __asm volatile ("wfi");
	}
}

#ifndef FEATURE_PERIPH_PM
void pm_set_lowest(void)
{
    /* The WFI will wake up even if interrupts are disabled. See WFI description. */
    if (read_csr(mstatus) & MSTATUS_MIE) {
    	while(1) {};
        //__asm volatile ("wfi");
    }
}
#endif

void pm_reboot(void)
{
	/* Watchdog runs at ~32KHz. Scaling is used to reduce the rate of ticks received. */
	/* Set the tick compare count value */
	AON_REG(AON_WDOGKEY)  = AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCMP)  = 1;  /* number of (scaled) ticks to wait */

	/* Set the config register, note scale is set to 0 */
	AON_REG(AON_WDOGKEY)  = AON_WDOGKEY_VALUE;
	AON_REG(AON_WDOGCFG)  = (1<<12)       /* wdogenalways - wdog runs always */
	                      | (1<< 8);      /* wdogrsten    - enable reset     */
	while(1);
}
