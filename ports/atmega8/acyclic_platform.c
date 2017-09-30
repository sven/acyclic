/**
 * @brief ACyCLIC - Auto Completition CommandLine InterfaCe
 *
 * A small commandline interface with autocompletition made for embedded
 * devices with low memory resources.
 *
 * Copyright (c) 2016, Sven Bachmann <dev@mcbachmann.de>
 *
 * Licensed under the MIT license, see LICENSE for details.
 */
#include <acyclic.h>


/*****************************************************************************/
/* Defines */
/*****************************************************************************/
#define BAUD 9600

/* http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_wrong_baud_rate */
#define UBRR_VAL ((F_CPU + BAUD * 8L) / (BAUD * 16L) - 1)


/*****************************************************************************/
/* Local variables */
/*****************************************************************************/
static ACYCLIC_T g_a;                           /**< ACyCLIC handle */
static uint8_t acyclic_mem[ACYCLIC_PLAT_MEMPOOL]; /**< memory pool */
static unsigned int acyclic_mem_pos = 0;        /**< memory pool position */


/*****************************************************************************/
/** Main
 *
 * @returns SHELL result
 */
int main(
    void
)
{
    /* initialize UART */
    UBRRH = (unsigned char) (UBRR_VAL >> 8);
    UBRRL = (unsigned char) (UBRR_VAL & 0xff);
    UCSRB = (1 << RXEN | 1 << TXEN);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    /* initialize ACyCLIC */
    memset(&g_a, 0, sizeof(ACYCLIC_T));
    acyclic_init(&g_a);

    /* handle input */
    while (!acyclic_flg_exit) {

        /* wait for character */
        while (!(UCSRA & (1 << RXC)));

        /* feed character to ACyCLIC */
        acyclic_input(&g_a, UDR);
    }

    return 0;
}


/*****************************************************************************/
/** Local mem allocator
 *
 * @retval ptr successful
 * @retval null failed
 */
void * acyclic_plat_calloc(
    unsigned int size                           /**< allocation size */
)
{
    uint8_t *ptr;
    uint8_t *ptr_end;

    if ((ACYCLIC_PLAT_MEMPOOL - acyclic_mem_pos) < size) {
        return NULL;
    }

    ptr = &acyclic_mem[acyclic_mem_pos];
    acyclic_mem_pos += size;
    ptr_end = &acyclic_mem[acyclic_mem_pos];

    for (; ptr_end >= ptr; ptr_end--) {
        *ptr_end = 0;
    }

    return ptr;
}


/*****************************************************************************/
/** Put character
 */
void acyclic_plat_putc(
    char c
)
{
    while (!(UCSRA & (1 << UDRE)));
    UDR = c;
}


/*****************************************************************************/
/** Put string
 */
void acyclic_plat_puts(
    const char *s
)
{
    for (; *s; s++) {
        acyclic_plat_putc(*s);
    }
}
