
/**
 * @ingroup     cpu_freedom-e
 * @ingroup     drivers_periph_uart
 * @{
 *
 * @file
 * @brief        Low-level UART driver implementation
 *
 * @}
 */

#include "cpu.h"
#include "sched.h"
#include "thread.h"
#include "bsp.h"
#include "platform.h"
#include "periph/uart.h"
#include "plic_driver.h"
#include <assert.h>



/**
 * @brief allocate memory to store callback functions
 */
static uart_isr_ctx_t ctx[UART_NUMOF];

void fe310_uart_irq_handler(plic_source num);


/*
static void uart_init(size_t baud_rate)
{
  GPIO_REG(GPIO_IOF_SEL) &= ~IOF0_UART0_MASK;
  GPIO_REG(GPIO_IOF_EN) |= IOF0_UART0_MASK;
  UART0_REG(UART_REG_DIV) = get_cpu_freq() / baud_rate - 1;
  UART0_REG(UART_REG_TXCTRL) |= UART_TXEN;
}
*/

/**
 * Clocks must be initialized prior to calling uart_init().
 *
 * UART initialization is not called from board_init but is called from the
 * newlib __libc_init_array C startup code.  Syscalls.c defines an _init()
 * function that results in uart initialization:
 * Start.s:_start() ->
 *   newlib::init.c:__libc_init_array() ->
 *     Syscalls.c:_init() ->
 *       uart_stdio.c:uart_stdio_init() ->
 *         uart_init()
 * I'm not sure why it's done this way...
 */
int uart_init(uart_t uart, uint32_t baudrate, uart_rx_cb_t rx_cb, void *arg)
{
	uint32_t stopbits = 1;

    /* make sure the uart device is valid */
    if (uart != 0) {
        return UART_NODEV;
    }

    /* save callbacks */
    ctx[uart].rx_cb = rx_cb;
    ctx[uart].arg = arg;

    /* Configure RX/TX pins */
    uint32_t mask = (1 << HIFIVE_UART0_TX) | (1 << HIFIVE_UART0_RX);
    GPIO_REG(GPIO_IOF_EN) |= mask;
    GPIO_REG(GPIO_IOF_SEL) &= ~mask;

    /* Install interrupt handler */
    PLIC_set_handler(INT_UART0_BASE, fe310_uart_irq_handler, 3);

    UART0_REG(UART_REG_DIV) = (get_cpu_freq() + (baudrate / 2)) / baudrate - 1;
    /* Set threshold and stop bits, not enable yet */
    UART0_REG(UART_REG_TXCTRL) = UART_TXWM(4) | ((stopbits - 1) << 1);

    /* Do not enable RX if there's no callback function */
    if (rx_cb == NULL) {
        /* RX enabled with interrupt when any bytes is there */
        UART0_REG(UART_REG_RXCTRL) = UART_RXWM(0) | UART_RXEN;
        UART0_REG(UART_REG_IE) = UART_IP_RXWM;

        PLIC_enable_interrupt(INT_UART0_BASE);
    }

    /* enable the transmitter */
	UART0_REG(UART_REG_TXCTRL) |= UART_TXEN;

    return UART_OK;
}

void uart_write(uart_t uart, const uint8_t *data, size_t len)
{
    if (uart != 0) {
        return;
    }

    for (size_t i = 0; i < len; i++) {
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
    	UART0_REG(UART_REG_TXFIFO) = data[i];
    }
}

void uart_poweron(uart_t uart)
{
	/* TODO: check if we can do a real power-down.*/
    assert(0);

    if (uart != 0) {
        return;
    }
}

void uart_poweroff(uart_t uart)
{
	/* TODO: check if we can do a real power-down.  Just disable interrupts for now. */
    if (uart != 0) {
        return;
    }

    UART0_REG(UART_REG_TXCTRL) &= ~UART_TXEN;
    UART0_REG(UART_REG_RXCTRL) = 0;
    PLIC_disable_interrupt(INT_UART0_BASE);
}

void fe310_uart_irq_handler(plic_source num)
{
    int data;

    data = UART0_REG(UART_REG_RXFIFO);
    assert(data >= 0);
    ctx[0].rx_cb(ctx[0].arg, (uint8_t)data);

    if (sched_context_switch_request) {
        thread_yield();
    }
}
