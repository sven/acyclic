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
/** Terminal initialisation
 */
int acyclic_term_init(
    ACYCLIC_T *a
)
{
    int res;
    struct termios term;

    /* store current terminal settings */
    res = tcgetattr(STDIN_FILENO, &a->plat.term);
    if (res) {
        fprintf(stderr, "Couldn't read termios attributes: %i (%s)\n", res, strerror(res));
        return res;
    }

    /* copy current settings to allow modification */
    term = a->plat.term;

    /* local modes
     * - disable canonical mode (line by line input)
     * - disable character echo
     */
    term.c_lflag &= ~(unsigned int) (ICANON | ECHO);

    /* set attributes */
    res = tcsetattr(STDIN_FILENO, TCSANOW, &term);
    if (res) {
        fprintf(stderr, "Couldn't set termios attributes: %i (%s)\n", res, strerror(res));
    }

    return res;
}


/*****************************************************************************/
/** Terminal exit
 */
int acyclic_term_exit(
    ACYCLIC_T *a
)
{
    int res;

    /* restore previous terminal settings */
    res = tcsetattr(STDIN_FILENO, TCSANOW, &a->plat.term);
    if (res) {
        fprintf(stderr, "Couldn't set termios attributes: %i (%s)\n", res, strerror(res));
    }

    return res;
}
