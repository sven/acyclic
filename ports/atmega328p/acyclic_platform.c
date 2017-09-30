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
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    UBRR0H = (UBRR_VAL >> 8);
    UBRR0L = UBRR_VAL;

    /* initialize ACyCLIC */
    memset(&g_a, 0, sizeof(ACYCLIC_T));
    acyclic_init(&g_a);

    /* handle input */
    while (!g_a.flg_exit) {

        /* wait for character */
        while (!(UCSR0A & (1 << RXC0)));

        /* feed character to ACyCLIC */
        acyclic_input(&g_a, UDR0);
    }

    return 0;
}


/*****************************************************************************/
/** Put character
 */
void acyclic_plat_putc(
    char c
)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
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
