/**
 * @brief ACyCLIC STDIO - Minimal printf Implementation
 *
 * Copyright (c) 2017, Sven Bachmann <dev@mcbachmann.de>
 *
 * Licensed under the MIT license, see LICENSE for details.
 */
#ifndef ACYCLIC_STDIO_H
#define ACYCLIC_STDIO_H


/*****************************************************************************/
/* Prototypes */
/*****************************************************************************/
void acyclic_stdio_printf(
    const char *fmt,                            /**< format string */
    ...                                         /**< variable arguments */
) __attribute__((format(printf, 1, 2)));


#endif /* ACYCLIC_STDIO_H */
