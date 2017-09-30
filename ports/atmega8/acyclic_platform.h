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
#ifndef ACYCLIC_PLATFORM_H
#define ACYCLIC_PLATFORM_H

#include <string.h>


/*****************************************************************************/
/* Platform defines */
/*****************************************************************************/
#include <avr/io.h>
#ifndef NULL
#  define NULL 0
#endif

#define ACYCLIC_PLAT_PUTC(x)            acyclic_plat_putc(x)
#define ACYCLIC_PLAT_PUTS(x)            acyclic_plat_puts(x)
#define ACYCLIC_PLAT_PRINTF(...)
#define ACYCLIC_PLAT_DBG_PRINTF(...)
#define ACYCLIC_PLAT_BACKSPACE()        ACYCLIC_PLAT_PUTC('\b'); ACYCLIC_PLAT_PUTC(' '); ACYCLIC_PLAT_PUTC('\b')
#define ACYCLIC_PLAT_NEWLINE()          ACYCLIC_PLAT_PUTC('\r'); ACYCLIC_PLAT_PUTC('\n')
#define ACYCLIC_PLAT_SPACE()            ACYCLIC_PLAT_PUTC(' ')
#define ACYCLIC_PLAT_PUTS_NL(x)         ACYCLIC_PLAT_PUTS(x); ACYCLIC_PLAT_NEWLINE()

#define ACYCLIC_PLAT_KEY_ENTER          13
#define ACYCLIC_PLAT_KEY_BS             8


/*****************************************************************************/
/* Prototypes */
/*****************************************************************************/
void acyclic_plat_putc(
    char c
);

void acyclic_plat_puts(
    const char *s
);


#endif /* ACYCLIC_PLATFORM_H */
