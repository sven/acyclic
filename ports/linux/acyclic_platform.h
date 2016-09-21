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
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <inttypes.h>

typedef struct {
    struct termios term;                        /**< terminal settings */
} ACYCLIC_PLATFORM_T;

int acyclic_term_init(
    struct ACYCLIC_T *a
);

int acyclic_term_exit(
    struct ACYCLIC_T *a
);

#define ACYCLIC_CALLOC(x)         calloc(1, x)
#define ACYCLIC_MEM_POOL          512

#define ACYCLIC_KEY_ENTER 10
#define ACYCLIC_KEY_BS    127
#define GETC(key) key = (uint8_t) getchar()
#define PUTC(x) putc(x, stdout)
#define ACYCLIC_MACRO_NEWLINE() PUTC('\n')
#define PUTS_INF(x) printf("%s", x)
#define PUTS_LINE_INF(x) printf("%s\n", x)
#define PRINTF printf
#define ACYCLIC_MACRO_SPACE() putc(' ', stdout)
#define ACYCLIC_MACRO_BACKSPACE() fputs("\b \b", stdout)

#if ACYCLIC_DBG > 0
#  define DBG_VARS FILE *fd;
#  define DBG_INIT() fd = fopen("acyclic.dbg", "a"); if (!fd) { exit(1); }
#  define DBG_PRINTF(...) fprintf(fd, __VA_ARGS__); fprintf(fd, "\n"); fflush(fd)
#  define DBG_CLOSE() fclose(fd)
extern FILE *fd;
#else
#  define DBG_VARS
#  define DBG_INIT()
#  define DBG_PRINTF(...)
#  define DBG_CLOSE()
#endif

#define ACYCLIC_TERM_INIT(x) acyclic_term_init(x)
#define ACYCLIC_TERM_EXIT(x) acyclic_term_exit(x)


#endif /* ACYCLIC_PLATFORM_H */
