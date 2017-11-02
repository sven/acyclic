/**
 * @brief ACyCLIC STDIO - Minimal printf Implementation
 *
 * Copyright (c) 2017, Sven Bachmann <dev@mcbachmann.de>
 *
 * Licensed under the MIT license, see LICENSE for details.
 */
#include <acyclic.h>
#include "acyclic_stdio.h"


/*****************************************************************************/
/* Prototypes */
/*****************************************************************************/
static void acyclic_stdio_printf_num(
    unsigned int num,                           /**< unsigned integer */
    unsigned int base,                          /**< base */
    uint8_t flg_zero,                           /**< prefix with zero */
    uint8_t cnt_pad                             /**< pad count */
);


/*****************************************************************************/
/** ACyCLIC Print An Unsigned Integer
 *
 * This function doesn't need an extra buffer to store the string-converted
 * integer but uses a lot of CPU power. It is a compromise because I didn't
 * found a header that contains the actual count of characters that is needed
 * to represent the platform specific integer as string. I didn't want a
 * catch-all solution that uses the same amount of reserved bytes for an 8-bit
 * system that is needed for an 64-bit integer.
 */
static void acyclic_stdio_printf_num(
    unsigned int num,                           /**< unsigned integer */
    unsigned int base,                          /**< base */
    uint8_t flg_zero,                           /**< prefix with zero */
    uint8_t cnt_pad                             /**< pad count */
)
{
    unsigned int num_temp;                      /* temporary number buffer */
    unsigned int digit;                         /* found digit */
    unsigned int digits = 0;                    /* digits found */
    unsigned int digit_cnt;                     /* digit counter */
    uint8_t pad;                                /* pad character */

    /* count digits */
    for (num_temp = num; num_temp; digit = num_temp % base, num_temp /= base, digits++);

    /* check if padding is needed */
    if (cnt_pad > digits) {
        pad = ((flg_zero) || (!digits)) ? '0' : ' ';
        for (cnt_pad -= digits; cnt_pad; cnt_pad--) {
            ACYCLIC_PLAT_PUTC(pad);
        }
    }

    for (; digits; digits--) {
        for (num_temp = num, digit_cnt = digits; digit_cnt; digit_cnt--, digit = num_temp % base, num_temp /= base);

        ACYCLIC_PLAT_PUTC(digit + ((digit < 10) ? '0' : ('a' - 10)));
    }
}


/*****************************************************************************/
/** ACyCLIC Just Enough Printf To Work
 *
 * Supports the following printf-features:
 *   - %s - string
 *   - %.*s - string with maximum length given as first parameter
 *   - %c - character
 *   - %u - unsigned integer printed as decimal
 *   - %x - unsigned integer printed as hex
 */
void acyclic_stdio_printf(
    const char *fmt,                            /**< format string */
    ...                                         /**< variable arguments */
)
{
    va_list ap;                                 /* variable argument list */
    unsigned int flg_format = 0;                /* format flag */
    char *sub;                                  /* sub string */
    uint8_t cnt_pad = 1;                        /* pad count */
    uint8_t flg_zero = 0;                       /* prefix with zero */
    uint8_t flg_precision = 0;                  /* precision flag */
    int precision = INT_MAX;                    /* precision */

    va_start(ap, fmt);
    for (; *fmt; fmt++) {

        if (flg_format) {

            if ('0' == *fmt) {
                flg_zero = 1;
                continue;
            }

            if (('1' <= *fmt) && ('9' >= *fmt)) {
                cnt_pad = *fmt - '0';
                continue;
            }

            if ((flg_precision) && ('*' == *fmt)) {
                precision = va_arg(ap, int);
                if (0 > precision) {
                    precision = 0;
                }
                continue;
            }

            if ('.' == *fmt) {
                flg_precision = 1;
                continue;
            }

            switch (*fmt) {
                case '%':
                    ACYCLIC_PLAT_PUTC('%');
                    break;

                case 's':
                    for (sub = va_arg(ap, char *); (precision--) && (*sub); sub++) {
                        ACYCLIC_PLAT_PUTC(*sub);
                    }
                    break;

                case 'c':
                    ACYCLIC_PLAT_PUTC((char) va_arg(ap, int));
                    break;

                case 'u':                       /* unsigned int */
                    acyclic_stdio_printf_num(va_arg(ap, unsigned int), 10, flg_zero, cnt_pad);
                    break;

                case 'x':                       /* unsigned int as hex */
                    acyclic_stdio_printf_num(va_arg(ap, unsigned int), 16, flg_zero, cnt_pad);
                    break;
            }

            cnt_pad = 1;
            flg_zero = 0;
            flg_format = 0;
            precision = INT_MAX;
            flg_precision = 0;

            continue;
        }

        if ('%' == *fmt) {
            flg_format = 1;
            continue;
        }

        ACYCLIC_PLAT_PUTC(*fmt);
    }
    va_end(ap);
}
