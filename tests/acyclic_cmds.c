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
#include "acyclic.h"


/*****************************************************************************/
/* Local variables */
/*****************************************************************************/
static ACYCLIC_T g_a = { 0 };                   /**< ACyCLIC handle */


/*****************************************************************************/
/* Local prototypes */
/*****************************************************************************/
uint8_t cmd_help_func(struct ACYCLIC_T *a);
uint8_t cmd_greet_func(struct ACYCLIC_T *a);
uint8_t cmd_exit_func(struct ACYCLIC_T *a);
uint8_t cmd_test_func(struct ACYCLIC_T *a);


/*****************************************************************************/
/* Commands */
/*****************************************************************************/
ACYCLIC_CMD(test_one, "one", NULL, NULL, NULL);
ACYCLIC_CMD(test, "test", NULL, &cmd_test_one, cmd_test_func);

ACYCLIC_CMD(sametext, "sametext", &cmd_test, NULL, NULL);
ACYCLIC_CMD(same, "same", &cmd_sametext, NULL, NULL);

ACYCLIC_CMD(rocky, "rocky", &cmd_same, NULL, NULL);
ACYCLIC_CMD(rockie, "rockie", &cmd_rocky, NULL, NULL);

ACYCLIC_CMD(help_speex, "speex", NULL, NULL, NULL);
ACYCLIC_CMD(help_smaxx, "smaxx", &cmd_help_speex, NULL, NULL);
ACYCLIC_CMD(help_snufz, "snufz", &cmd_help_smaxx, NULL, NULL);
ACYCLIC_CMD(help_greet, "greet", &cmd_help_snufz, NULL, NULL);
ACYCLIC_CMD(help, "help", &cmd_rockie, &cmd_help_greet, cmd_help_func);

ACYCLIC_CMD(greet_me, "me", NULL, NULL, NULL);
ACYCLIC_CMD(greet, "greet", &cmd_help, &cmd_greet_me, cmd_greet_func);

ACYCLIC_CMD(exit, "exit", &cmd_greet, NULL, cmd_exit_func);


/*****************************************************************************/
/** Test Main
 *
 * @returns SHELL result
 */
int main(
    void
)
{
    int res;                                    /* result */

    /* initialize platform */
    res = acyclic_plat_init();

    /* initialize ACyCLIC */
    if (!res) {
        res = acyclic_init(&g_a);
    }

    /* register commands */
    if (!res) {
        res = acyclic_cmd_add(&g_a, &g_a.cmds, &cmd_exit);
    }

    /* handle input */
    if (!res) {
        while (!g_a.flg_exit) {
            acyclic_input(&g_a, (uint8_t) ACYCLIC_PLAT_GETC());
        }
    }

    /* shutdown platform */
    acyclic_plat_exit();

    return res;
}


/*****************************************************************************/
/** Help
 */
uint8_t cmd_help_func(
    struct ACYCLIC_T *a
)
{
    ACYCLIC_UNUSED(a);

    ACYCLIC_PLAT_PUTC('\n');
    ACYCLIC_PLAT_PRINTF("********\n");
    ACYCLIC_PLAT_PRINTF("* help *\n");
    ACYCLIC_PLAT_PRINTF("********\n");
    ACYCLIC_PLAT_PRINTF("[help]   greet - show greeting\n");
    ACYCLIC_PLAT_PUTC('\n');

    return 0;
}


/*****************************************************************************/
/** Test
 */
uint8_t cmd_test_func(
    struct ACYCLIC_T *a
)
{
    unsigned int cnt_arg;

    if (a->arg_cnt) {
        ACYCLIC_PLAT_PRINTF("args: ");
        for (cnt_arg = 0; cnt_arg < a->arg_cnt; cnt_arg++) {
            ACYCLIC_PLAT_PUTC('[');
            if (a->args[cnt_arg].cmd) {
                ACYCLIC_PLAT_PUTC('*');
            }
            ACYCLIC_PLAT_PRINTF("%.*s", a->args[cnt_arg].len, a->args[cnt_arg].name);
            ACYCLIC_PLAT_PRINTF("] ");
        }
        ACYCLIC_PLAT_PUTC('\n');
    }

    return 0;
}


/*****************************************************************************/
/** Greet
 */
uint8_t cmd_greet_func(
    struct ACYCLIC_T *a
)
{
#if ACYCLIC_DBG == 0
    ACYCLIC_UNUSED(a);
#endif
    uint8_t cnt;

    ACYCLIC_PLAT_PUTC('\n');
    ACYCLIC_PLAT_PRINTF("*********\n");
    ACYCLIC_PLAT_PRINTF("* greet *\n");
    ACYCLIC_PLAT_PRINTF("*********\n");
    ACYCLIC_PLAT_PRINTF("[greet]  hello from me\n");
    ACYCLIC_PLAT_PRINTF("[greet]  arguments:\n");

    for (cnt = 0; cnt < a->arg_cnt; cnt++) {
        ACYCLIC_PLAT_PRINTF("[greet]  arg: %.*s\n", a->args[cnt].len, a->args[cnt].name);
    }

    ACYCLIC_PLAT_PUTC('\n');

    return 0;
}


/*****************************************************************************/
/** Exit
 */
uint8_t cmd_exit_func(
    struct ACYCLIC_T *a
)
{
    ACYCLIC_UNUSED(a);

    a->flg_exit = 1;

    ACYCLIC_PLAT_PUTC('\n');
    ACYCLIC_PLAT_PRINTF("********\n");
    ACYCLIC_PLAT_PRINTF("* exit *\n");
    ACYCLIC_PLAT_PRINTF("********\n");
    ACYCLIC_PLAT_PRINTF("[exit]  bye\n");
    ACYCLIC_PLAT_PUTC('\n');

    return 0;
}
