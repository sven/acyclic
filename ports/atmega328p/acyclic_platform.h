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

#include <acyclic_stdio.h>


/*****************************************************************************/
/* Platform defines */
/*****************************************************************************/
#define ACYCLIC_PLAT_GETC()             acyclic_plat_getc()
#define ACYCLIC_PLAT_PUTC(x)            acyclic_plat_putc(x)
#define ACYCLIC_PLAT_PRINTF(...)        acyclic_stdio_printf(__VA_ARGS__)
#define ACYCLIC_PLAT_DBG_PRINTF(...)

#define acyclic_plat_exit()


/*****************************************************************************/
/* Prototypes */
/*****************************************************************************/
int acyclic_plat_init(
    void
);

char acyclic_plat_getc(
    void
);

void acyclic_plat_putc(
    char c
);


#endif /* ACYCLIC_PLATFORM_H */
