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

#include <stdio.h>


/*****************************************************************************/
/* Defines */
/*****************************************************************************/
#define ACYCLIC_PLAT_GETC()             getchar()
#define ACYCLIC_PLAT_PUTC(x)            putchar(x)
#define ACYCLIC_PLAT_PRINTF(...)        printf(__VA_ARGS__)

#if ACYCLIC_DBG > 0
extern FILE *acyclic_plat_dbg_fd;       /**< debug file desc */
#  define ACYCLIC_PLAT_DBG_PRINTF(...)  fprintf(acyclic_plat_dbg_fd, __VA_ARGS__); fprintf(acyclic_plat_dbg_fd, "\n"); fflush(acyclic_plat_dbg_fd)
#else
#  define ACYCLIC_PLAT_DBG_PRINTF(...)
#endif


/*****************************************************************************/
/* Prototypes */
/*****************************************************************************/
int acyclic_plat_init(
    void
);

int acyclic_plat_exit(
    void
);


#endif /* ACYCLIC_PLATFORM_H */
