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
typedef struct {
} ACYCLIC_PLATFORM_T;

#include <avr/io.h>
#ifndef NULL
#  define NULL 0
#endif

#define ACYCLIC_CALLOC(x)         acyclic_calloc(x)
#define ACYCLIC_CALLOC_INTERNAL   1
#define ACYCLIC_MEM_POOL          256

#define ACYCLIC_KEY_ENTER 13
#define ACYCLIC_KEY_BS    8
#define GETC(key) while (!(UCSRA & (1 << RXC))); key = UDR
#define PUTC(x) while (!(UCSRA & (1 << UDRE))); UDR = x
#define ACYCLIC_MACRO_NEWLINE() PUTC('\r'); PUTC('\n')
#define PUTS_INF(x)       { \
                                const char *s = x;                                  \
                                while (*s) {                                        \
                                    PUTC(*s);                                       \
                                    s++;                                            \
                                }                                                   \
                            }
#define PRINTF(...)
#define ACYCLIC_MACRO_BACKSPACE() PUTC('\b'); PUTC(' '); PUTC('\b')
#define DBG_VARS
#define DBG_INIT()
#define DBG_PRINTF(...)
#define DBG_CLOSE()
#define PUTS_LINE_INF(x)  { \
                                char *s = x;                                        \
                                while (*s) {                                        \
                                    PUTC(*s);                                       \
                                    s++;                                            \
                                }                                                   \
                                PUTC('\r');                                         \
                                PUTC('\n');                                         \
                            }

#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)
#define ACYCLIC_TERM_INIT(x) { \
                                UBRRH = (unsigned char) (UBRR_VAL >> 8);            \
                                UBRRL = (unsigned char) (UBRR_VAL & 0xff);          \
                                UCSRB = (1 << RXEN | 1 << TXEN);                    \
                                UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); \
                            }
#define ACYCLIC_TERM_EXIT(x)
#define ACYCLIC_MACRO_SPACE() PUTC(' ')


#endif /* ACYCLIC_PLATFORM_H */
