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
#include <avr/io.h>
#include <acyclic.h>


/*****************************************************************************/
/* Defines */
/*****************************************************************************/
#define BAUD 9600

/* http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_wrong_baud_rate */
#define UBRR_VAL ((F_CPU + BAUD * 8L) / (BAUD * 16L) - 1)


/*****************************************************************************/
/** Initialize platform
 *
 * @returns SHELL result
 */
int acyclic_plat_init(
    void
)
{
    /* initialize UART */
    UBRRH = (unsigned char) (UBRR_VAL >> 8);
    UBRRL = (unsigned char) (UBRR_VAL & 0xff);
    UCSRB = (1 << RXEN | 1 << TXEN);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    return 0;
}


/*****************************************************************************/
/** Read character
 */
char acyclic_plat_getc(
    void
)
{
    while (!(UCSRA & (1 << RXC)));
    return UDR;
}


/*****************************************************************************/
/** Put character
 */
void acyclic_plat_putc(
    char c
)
{
    if ('\n' == c) {
        while (!(UCSRA & (1 << UDRE)));
        UDR = '\r';
    }

    while (!(UCSRA & (1 << UDRE)));
    UDR = c;
}
