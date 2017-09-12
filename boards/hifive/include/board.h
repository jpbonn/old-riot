/*
 * Copyright (C) 2016 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    boards_hifive SiFive HiFive1
 * @ingroup     boards
 * @brief       Board specific files for the SiFive HiFive1
 * @{
 *
 * @file
 * @brief       Board specific configuration for the SiFive HiFive1
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initialize the board, also triggers the CPU initialization
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
