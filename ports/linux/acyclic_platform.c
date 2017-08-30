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
/* Global variables */
/*****************************************************************************/
#if ACYCLIC_DBG > 0
FILE *acyclic_plat_dbg_fd;                      /**< debug file desc */
#endif


/*****************************************************************************/
/* Local variables */
/*****************************************************************************/
static ACYCLIC_T g_a;                           /**< ACyCLIC handle */
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
/** Main
 *
 * @returns SHELL result
 */
int main(
    void
)
{
    /* initialize terminal */
    plat_term_init(&g_term);

#if ACYCLIC_DBG > 0
    /* initialize debugging */
    acyclic_plat_dbg_fd = fopen("acyclic.dbg", "a");
    if (!acyclic_plat_dbg_fd) {
        exit(1);
    }

    ACYCLIC_PLAT_DBG_PRINTF("\n\nacyclic started\n");
#endif

    /* initialize ACyCLIC */
    memset(&g_a, 0, sizeof(ACYCLIC_T));
    acyclic_init(&g_a);

    /* handle input */
    while (!acyclic_flg_exit) {
        acyclic_input(&g_a, (uint8_t) getchar());
    }

#if ACYCLIC_DBG > 0
    /* close debugging */
    fclose(acyclic_plat_dbg_fd);
#endif

    /* close terminal */
    plat_term_exit(&g_term);
}


/*****************************************************************************/
/** Terminal initialisation
 */
static int plat_term_init(
    struct termios *term                        /**< terminal data */
)
{
    int res;
    struct termios term_tmp;

    /* store current terminal settings */
    res = tcgetattr(STDIN_FILENO, term);
    if (res) {
        fprintf(stderr, "Couldn't read termios attributes: %i (%s)\n", res, strerror(res));
        return res;
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
        fprintf(stderr, "Couldn't set termios attributes: %i (%s)\n", res, strerror(res));
    }

    return res;
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
        fprintf(stderr, "Couldn't set termios attributes: %i (%s)\n", res, strerror(res));
    }

    return res;
}
