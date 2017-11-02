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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <inttypes.h>


/*****************************************************************************/
/* Global variables */
/*****************************************************************************/
#if ACYCLIC_DBG > 0
FILE *acyclic_plat_dbg_fd;                      /**< debug file desc */
#endif


/*****************************************************************************/
/* Local variables */
/*****************************************************************************/
static struct termios g_term;                   /**< terminal settings */


/*****************************************************************************/
/* Local prototypes */
/*****************************************************************************/
static int plat_term_init(
    struct termios *term                        /**< terminal data */
);

static int plat_term_exit(
    struct termios *term                        /**< terminal data */
);


/*****************************************************************************/
/** Initialize platform
 *
 * @returns SHELL result
 */
int acyclic_plat_init(
    void
)
{
    int res;                                    /* result */

    /* initialize terminal */
    res = plat_term_init(&g_term);
    if (res) {
        return res;
    }

#if ACYCLIC_DBG > 0
    /* initialize debugging */
    acyclic_plat_dbg_fd = fopen("acyclic.dbg", "a");
    if (!acyclic_plat_dbg_fd) {
        fprintf(stderr, "Couldn't open debug file: %i (%s)\n", errno, strerror(errno));
        return 1;
    }

    ACYCLIC_PLAT_DBG_PRINTF("\n\nacyclic started\n");
#endif

    return res;
}


/*****************************************************************************/
/** Shutdown platform
 *
 * @returns SHELL result
 */
int acyclic_plat_exit(
    void
)
{
#if ACYCLIC_DBG > 0
    /* close debugging */
    fclose(acyclic_plat_dbg_fd);
#endif

    /* close terminal */
    plat_term_exit(&g_term);

    return 0;
}


/*****************************************************************************/
/** Terminal initialisation
 */
static int plat_term_init(
    struct termios *term                        /**< terminal data */
)
{
    int res;                                    /* result */
    struct termios term_tmp;                    /* temporary termios data */

    /* store current terminal settings */
    res = tcgetattr(STDIN_FILENO, term);
    if (res) {
        fprintf(stderr, "Couldn't read termios attributes: %i (%s)\n", errno, strerror(errno));
        return 1;
    }

    /* copy current settings to allow modification */
    term_tmp = *term;

    /* local modes
     * - disable canonical mode (line by line input)
     * - disable character echo
     */
    term_tmp.c_lflag &= ~(unsigned int) (ICANON | ECHO);

    /* set attributes */
    res = tcsetattr(STDIN_FILENO, TCSANOW, &term_tmp);
    if (res) {
        fprintf(stderr, "Couldn't set termios attributes: %i (%s)\n", errno, strerror(errno));
        return 1;
    }

    return 0;
}


/*****************************************************************************/
/** Terminal exit
 */
static int plat_term_exit(
    struct termios *term                        /**< terminal data */
)
{
    int res;

    /* restore previous terminal settings */
    res = tcsetattr(STDIN_FILENO, TCSANOW, term);
    if (res) {
        fprintf(stderr, "Couldn't set termios attributes: %i (%s)\n", errno, strerror(errno));
        return 1;
    }

    return 0;
}
