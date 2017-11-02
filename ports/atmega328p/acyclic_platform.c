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
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    UBRR0H = (UBRR_VAL >> 8);
    UBRR0L = UBRR_VAL;

    return 0;
}


/*****************************************************************************/
/** Read character
 */
char acyclic_plat_getc(
    void
)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}


/*****************************************************************************/
/** Put character
 */
void acyclic_plat_putc(
    char c
)
{
    if ('\n' == c) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '\r';
    }

    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}
