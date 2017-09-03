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
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <inttypes.h>


/*****************************************************************************/
/* Defines */
/*****************************************************************************/
#define ACYCLIC_PLAT_CALLOC(x)          calloc(1, x)
#define ACYCLIC_PLAT_PUTC(x)            putchar(x)
#define ACYCLIC_PLAT_PUTS(x)            fputs(x, stdout)
#define ACYCLIC_PLAT_PRINTF(...)        printf(__VA_ARGS__)
#define ACYCLIC_PLAT_NEWLINE()          ACYCLIC_PLAT_PUTC('\n')
#define ACYCLIC_PLAT_SPACE()            ACYCLIC_PLAT_PUTC(' ')
#define ACYCLIC_PLAT_BACKSPACE()        ACYCLIC_PLAT_PUTS("\b \b")
#define ACYCLIC_PLAT_PUTS_NL(x)         ACYCLIC_PLAT_PUTS(x); ACYCLIC_PLAT_NEWLINE()

#define ACYCLIC_PLAT_KEY_ENTER          10
#define ACYCLIC_PLAT_KEY_BS             127

#if ACYCLIC_DBG > 0
extern FILE *acyclic_plat_dbg_fd;       /**< debug file desc */
#  define ACYCLIC_PLAT_DBG_PRINTF(...)  fprintf(acyclic_plat_dbg_fd, __VA_ARGS__); fprintf(acyclic_plat_dbg_fd, "\n"); fflush(acyclic_plat_dbg_fd)
#else
#  define ACYCLIC_PLAT_DBG_PRINTF(...)
#endif


/*****************************************************************************/
/* Global variables */
/*****************************************************************************/
extern struct ACYCLIC_T *g_a;                   /**< ACyCLIC handle ptr */


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
