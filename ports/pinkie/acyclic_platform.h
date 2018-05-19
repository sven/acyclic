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

#include <pinkie_stdio.h>
#include <pinkie_arch.h>


/*****************************************************************************/
/* Defines */
/*****************************************************************************/
#define ACYCLIC_PLAT_PUTC               pinkie_stdio_putc
#define ACYCLIC_PLAT_PRINTF             pinkie_printf


#endif /* ACYCLIC_PLATFORM_H */
