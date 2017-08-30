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
/* Local prototypes */
/*****************************************************************************/
uint8_t cmd_help_func(struct ACYCLIC_T *a);
uint8_t cmd_greet_func(struct ACYCLIC_T *a);
uint8_t cmd_exit_func(struct ACYCLIC_T *a);
uint8_t cmd_test_func(struct ACYCLIC_T *a);


/*****************************************************************************/
/* Local variables */
/*****************************************************************************/
#define CMD_HELP        "help"
#define CMD_ROCKY       "rocky"
#define CMD_ROCKIE      "rockie"
#define CMD_GREET       "greet"
#define CMD_SPEEX       "speex"
#define CMD_SMAXX       "smaxx"
#define CMD_SNUFZ       "snufz"
#define CMD_SAME        "same"
#define CMD_SAMETEXT    "sametext"
#define CMD_EXIT        "exit"
#define CMD_ME          "me"
#define CMD_TEST        "test"
#define CMD_TEST_ONE    "one"


uint8_t cmd_help_func(
    struct ACYCLIC_T *a
)
{
    ACYCLIC_UNUSED(a);

    ACYCLIC_PLAT_NEWLINE();
    ACYCLIC_PLAT_PUTS_NL("********");
    ACYCLIC_PLAT_PUTS_NL("* help *");
    ACYCLIC_PLAT_PUTS_NL("********");
    ACYCLIC_PLAT_PUTS_NL("[help]   greet - show greeting");
    ACYCLIC_PLAT_NEWLINE();

    return 0;
}


uint8_t cmd_test_func(
    struct ACYCLIC_T *a
)
{
    unsigned int cnt_arg;
    unsigned int pos_arg;

    if (a->arg_cnt) {
        ACYCLIC_PLAT_PUTS("args: ");
        for (cnt_arg = 0; cnt_arg < a->arg_cnt; cnt_arg++) {
            ACYCLIC_PLAT_PUTC('[');
            if (a->args[cnt_arg].cmd) {
                ACYCLIC_PLAT_PUTC('*');
            }
            for (pos_arg = 0; pos_arg < a->args[cnt_arg].len; pos_arg++) {
                ACYCLIC_PLAT_PUTC(a->args[cnt_arg].name[pos_arg]);
            }
            ACYCLIC_PLAT_PUTS("] ");
        }
        ACYCLIC_PLAT_NEWLINE();
    }

    return 0;
}


uint8_t cmd_greet_func(
    struct ACYCLIC_T *a
)
{
#if ACYCLIC_DBG == 0
    ACYCLIC_UNUSED(a);
#endif
    uint8_t cnt;

    ACYCLIC_PLAT_NEWLINE();
    ACYCLIC_PLAT_PUTS_NL("*********");
    ACYCLIC_PLAT_PUTS_NL("* greet *");
    ACYCLIC_PLAT_PUTS_NL("*********");
    ACYCLIC_PLAT_PUTS_NL("[greet]  hello from me");
    ACYCLIC_PLAT_PUTS_NL("[greet]  arguments:");

    for (cnt = 0; cnt < a->arg_cnt; cnt++) {
        ACYCLIC_PLAT_PRINTF("[greet]  arg: %.*s\n", a->args[cnt].len, a->args[cnt].name);
    }

    ACYCLIC_PLAT_NEWLINE();

    return 0;
}


uint8_t cmd_exit_func(
    struct ACYCLIC_T *a
)
{
    ACYCLIC_UNUSED(a);

    acyclic_flg_exit = 1;

    ACYCLIC_PLAT_NEWLINE();
    ACYCLIC_PLAT_PUTS_NL("*******");
    ACYCLIC_PLAT_PUTS_NL("* exit *");
    ACYCLIC_PLAT_PUTS_NL("********");
    ACYCLIC_PLAT_PUTS_NL("[exit]  bye");
    ACYCLIC_PLAT_NEWLINE();

    return 0;
}


int acyclic_cmd_reg(
    struct ACYCLIC_T *a
)
{
    int res;
    ACYCLIC_CMD_T *elem;

    /* help */
    res = acyclic_cmd_add(a, &a->cmds, CMD_HELP, cmd_help_func, &elem);

    /* help: greet, speex, smaxx, snufz */
    res |= acyclic_cmd_add(a, &elem->sub, CMD_GREET, NULL, NULL);
    res |= acyclic_cmd_add(a, &elem->sub, CMD_SPEEX, NULL, NULL);
    res |= acyclic_cmd_add(a, &elem->sub, CMD_SMAXX, NULL, NULL);
    res |= acyclic_cmd_add(a, &elem->sub, CMD_SNUFZ, NULL, NULL);

    /* test */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_TEST, cmd_test_func, &elem);

    /* test: one */
    res |= acyclic_cmd_add(a, &elem->sub, CMD_TEST_ONE, NULL, NULL);

    /* greet */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_GREET, cmd_greet_func, &elem);

    /* greet: me */
    res |= acyclic_cmd_add(a, &elem->sub, CMD_ME, NULL, NULL);

    /* rocky */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_ROCKY, NULL, NULL);

    /* rockie */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_ROCKIE, NULL, NULL);

    /* same */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_SAME, NULL, NULL);

    /* sametext */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_SAMETEXT, NULL, NULL);

    /* exit */
    res |= acyclic_cmd_add(a, &a->cmds, CMD_EXIT, cmd_exit_func, NULL);

    return res;
}
