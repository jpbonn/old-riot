

/**
 * @defgroup    cpu_freedom-e Freedom E
 * @ingroup     cpu
 * @brief       Freedom E cpu
 * @{
 *
 * @file
 * @brief       Common implementations and headers for RISC V cpu
 *
 * @author      JP Bonn
 */

#ifndef CPU_H
#define CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <inttypes.h>

/**
 * @brief   Print the last instruction's address
 *
 * @todo:   Not supported
 */
static inline void cpu_print_last_instruction(void)
{
    /* This function must exist else RIOT won't compile */
}

#ifdef __cplusplus
}
#endif

#endif /* CPU_H */
/** @} */
