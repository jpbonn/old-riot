/*
 * Copyright (C) 2014 Freie Universität Berlin, Hinnerk van Bruinehsen
 * Copyright (C) 2016 INRIA, Francisco Acosta
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_hifive
 * @{
 *
 * @file
 * @brief       Peripheral MCU configuration for the SiFive HiFive1 board
 *
 */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    UART configuration
 *
 *          The CPU only supports one UART device, so we keep it simple
 * @{
 */
#define UART_NUMOF          (1U)
/** @} */


#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CONF_H */
