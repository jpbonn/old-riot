/*
 * Copyright(C) 2016,2017, Imagination Technologies Limited and/or its
 *                 affiliated group companies.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/**
 * @defgroup    cpu_freedom-e Freedom E
 * @ingroup     cpu
 * @{
 *
 * @file
 * @brief       CPU definitions for Freedom-e devices.
 *
 * @author      JP Bonn
 */

#ifndef CPU_CONF_H
#define CPU_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Configuration of default stack sizes
 *
 * These values were arbitrarily chosen.
 *
 * If needed, you can overwrite these values the the `cpu_conf.h` file of the
 * specific CPU implementation.
 *
 * @todo See if these values are reasonable
 * @{
 */
#ifndef THREAD_EXTRA_STACKSIZE_PRINTF
#define THREAD_EXTRA_STACKSIZE_PRINTF   (512)
#endif
#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT        (1024)
#endif
#ifndef THREAD_STACKSIZE_IDLE
#define THREAD_STACKSIZE_IDLE           (256)
#endif
/** @} */

/**
 * @brief   Stack size used for the exception (ISR) stack
 * @{
 * @todo NOT CURRENTLY USING
 *
 */
#ifndef ISR_STACKSIZE
#define ISR_STACKSIZE                   (512U)
#endif
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CPU_CONF_H */
/** @} */
