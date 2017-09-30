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
static void acyclic_space_skip(
    char **str,                                 /**< parser string */
    unsigned int *len                           /**< string length */
);

static void acyclic_cmd_show(
    ACYCLIC_T *a,                               /**< instance handle */
    ACYCLIC_CMD_T *cmd,                         /**< commands */
    uint8_t flg_show_all                        /**< show all flag */
);

static void acyclic_ac(
    ACYCLIC_T *a                                /**< instance handle */
);

static void acyclic_bs(
    ACYCLIC_T *a                                /**< instance handle */
);

static uint8_t acyclic_substr(
    ACYCLIC_T *a,                               /**< instance handle */
    char *sub,                                  /**< substring */
    unsigned int *sub_len,                      /**< substring length */
    unsigned int *len                           /**< overall length */
);

static void acyclic_key(
    ACYCLIC_T *a                                /**< instance handle */
);

static void acyclic_enter(
    ACYCLIC_T *a                                /**< instance handle */
);

static void acyclic_cmdline(
    ACYCLIC_T *a                                /**< instance handle */
);

static uint8_t acyclic_cmp(
    const char *first,                          /**< first string */
    const char *second,                         /**< second string */
    unsigned int len_first,                     /**< first length */
    unsigned int len_second                     /**< second length */
);

static uint8_t acyclic_srch(
    uint8_t flg_ac,                             /**< ac flag */
    char *sub,                                  /**< substring */
    unsigned int len_sub,                       /**< substring length */
    ACYCLIC_CMD_T *cmds,                        /**< command list */
    ACYCLIC_CMD_T **cmds_found                  /**< list of found commands */
);

static void acyclic_char_add(
    ACYCLIC_T *a,                               /**< instance handle */
    char chr                                    /**< character */
);

static int acyclic_key_pre(
    ACYCLIC_T *a                                /**< instance handle */
);

#if ACYCLIC_HISTORY == 1
static void acyclic_history_push(
    ACYCLIC_T *a                                /**< instance handle */
);

static void acyclic_history_pop(
    ACYCLIC_T *a                                /**< instance handle */
);

static void acyclic_history_up(
    ACYCLIC_T *a                                /**< instance handle */
);

static void acyclic_history_down(
    ACYCLIC_T *a                                /**< instance handle */
);

#  if ACYCLIC_HISTORY_DUMP == 1
static void acyclic_history_dump(
    ACYCLIC_T *a                                /**< instance handle */
);
#  endif
#endif

#if (ACYCLIC_HISTORY == 0) || (ACYCLIC_HISTORY_DUMP == 0)
#  define acyclic_history_dump(a)
#endif


/*****************************************************************************/
/* Local variables */
/*****************************************************************************/
uint8_t acyclic_flg_exit = 0;                   /**< exit flag */


/*****************************************************************************/
/** ACyCLIC init
 *
 * The handle pointer must point to a zeroed memory location.
 */
int acyclic_init(
    ACYCLIC_T *a                                /**< ACyCLIC handle */
)
{
    /* initialize acyclic data */
    a->flg_prompt = 1;

#if ACYCLIC_HISTORY == 1
    /* initialize history pointer */
    a->hist = &a->cmdline[ACYCLIC_CMDLINE_LEN];
#endif

    /* register commands */
    if (acyclic_cmd_reg(a)) {
        return 1;
    }

    /* greeting */
    ACYCLIC_PLAT_PUTS_NL("acyclic started");

    /* show current commandline */
    acyclic_cmdline(a);

    return 0;
}


/*****************************************************************************/
/** Character input
 */
void acyclic_input(
    ACYCLIC_T *a,                               /**< instance handle */
    uint8_t key                                 /**< input */
)
{
    /* store character */
    a->key = key;
    ACYCLIC_PLAT_DBG_PRINTF("[getc]   key: 0x%02x, %d", a->key, a->key);

    /* handle meta sequences */
    if (acyclic_key_pre(a)) {
        return;
    }

#if ACYCLIC_HISTORY == 1
    /* if in scroll mode, update current command */
    if (a->hist_scroll_cnt) {
        acyclic_history_pop(a);
        acyclic_history_dump(a);
    }
#endif

    /* check for tabulator */
    if (ACYCLIC_KEY_TAB == a->key) {
        if (a->cnt_tab < 2) {
            a->cnt_tab++;
        }
    } else {
        a->cnt_tab = 0;
    }

    /* check control characters */
    switch (a->key) {

        case ACYCLIC_PLAT_KEY_BS:
            acyclic_bs(a);
            break;

        case ACYCLIC_KEY_TAB:
            ACYCLIC_PLAT_DBG_PRINTF("[tab]    a->cmdline_len: %3u", a->cmdline_len);

            /* reset application arguments */
            a->arg_cnt = 0;

            acyclic_ac(a);
            break;

        case ACYCLIC_PLAT_KEY_ENTER:
            ACYCLIC_PLAT_DBG_PRINTF("[enter]  a->cmdline_len: %3u | a->cmdline: %.*s", a->cmdline_len, a->cmdline_len, a->cmdline);
            acyclic_enter(a);
            break;

        default:
            acyclic_key(a);
    }

    /* dump history state if enabled */
    acyclic_history_dump(a);

    /* show current commandline */
    acyclic_cmdline(a);
}


/*****************************************************************************/
/** Get command match or list of AC targets
 *
 * @retval ACYCLIC_SRCH_FOUND
 * @retval ACYCLIC_SRCH_AC
 * @retval ACYCLIC_SRCH_NONE
 */
static uint8_t acyclic_srch(
    uint8_t flg_ac,                             /**< ac flag */
    char *sub,                                  /**< substring */
    unsigned int len_sub,                       /**< substring length */
    ACYCLIC_CMD_T *cmds,                        /**< command list */
    ACYCLIC_CMD_T **cmds_found                  /**< list of found commands */
)
{
    uint8_t res_cmp;
    ACYCLIC_CMD_T *cmd;
    ACYCLIC_CMD_T *cmd_last = NULL;
    uint8_t flg_match_full = 0;

    *cmds_found = NULL;

    /* if no commands are available, return full match */
    if (!cmds) {
        return ACYCLIC_SRCH_NONE;
    }

    for (cmd = cmds; cmd; cmd = cmd->next) {

        res_cmp = acyclic_cmp(sub, cmd->name, len_sub, cmd->len);
        switch (res_cmp) {

            case ACYCLIC_CMP_MATCH:
                flg_match_full = 1;

            case ACYCLIC_CMP_END_FIRST:
                /* found AC match */
                if (!*cmds_found) {
                    *cmds_found = cmd;
                    cmd_last = cmd;
                } else {
                    cmd_last->next_ac = cmd;
                    cmd_last = cmd;
                }
                cmd->next_ac = NULL;
                break;
        }

        /* check if a single full match was found */
        if ((flg_match_full) && (!flg_ac)) {
            return ACYCLIC_SRCH_FOUND;
        }
    }

    return (*cmds_found) ? ACYCLIC_SRCH_AC : ACYCLIC_SRCH_NONE;
}


/*****************************************************************************/
/** Autocomplete command
 */
static void acyclic_ac(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    unsigned int cmdline_len = a->cmdline_len;
    uint8_t res_sub = 0;
    uint8_t res_srch = 0;
    unsigned int cnt;
    ACYCLIC_CMD_T *cmd = a->cmds;
    ACYCLIC_CMD_T *cmds_found = NULL;
    char *arg = a->cmdline;
    unsigned int arg_len;

    /* stop at string end */
    while (cmdline_len) {

        /* check if argument can be stored */
        if (ACYCLIC_ARG_COUNT <= a->arg_cnt) {
            if (0 == a->cnt_tab) {
                ACYCLIC_PLAT_PUTS_NL("max arg count reached");
                a->args[0].cmd = NULL;
                a->flg_cmd_show = 1;
            }

            /* end parsing */
            return;
        }

        /* remove leading spaces */
        acyclic_space_skip(&arg, &cmdline_len);
        if (!cmdline_len) {
            break;
        }

        /* find and skip substring */
        res_sub = acyclic_substr(a, arg, &arg_len, &cmdline_len);

        /* find substring in commands */
        res_srch = acyclic_srch(a->cnt_tab && (ACYCLIC_SUBSTR_OPEN == res_sub), arg, arg_len, cmd, &cmds_found);

        /* tab */
        if (a->cnt_tab && (ACYCLIC_SUBSTR_OPEN == res_sub)) {
            if (ACYCLIC_SRCH_AC == res_srch) {

                /* more than one result found, need double tab to show */
                if (cmds_found->next_ac) {
                    acyclic_cmd_show(a, cmds_found, 0);
                }
                /* only one result found, do autocomplete */
                else {
                    for (cnt = arg_len; cnt < cmds_found->len; cnt++) {
                        acyclic_char_add(a, cmds_found->name[cnt]);
                    }

                    /* update search result */
                    res_srch = ACYCLIC_SRCH_FOUND;
                }
            }

            /* word complete, add space */
            if (ACYCLIC_SRCH_FOUND == res_srch) {
                acyclic_char_add(a, ' ');
            }

            /* no further action needed here */
            return;
        }

        /* no tab requested, need full match on first argument */
        if (0 == a->arg_cnt) {
            if (ACYCLIC_SRCH_FOUND != res_srch) {
                if (0 == a->cnt_tab) {
                    if (acyclic_cmd_not_found(a)) {
                        ACYCLIC_PLAT_PUTS_NL("command not found");
                    }
                    a->flg_cmd_show = 1;
                }

                /* end parsing */
                return;
            }
        }

        /* update found arguments */
        if (cmds_found && (cmds_found->len == arg_len)) {
            a->args[a->arg_cnt].name = cmds_found->name;
            a->args[a->arg_cnt].cmd = cmds_found;

            if (cmds_found->func) {
                a->func = cmds_found->func;
            }
        } else {
            a->args[a->arg_cnt].name = arg;
            a->args[a->arg_cnt].cmd = NULL;
        }
        a->args[a->arg_cnt].len = arg_len;
        a->arg_cnt++;

        /* next argument */
        arg += arg_len;

        /* argument was found, update command list for following parameters */
        if (cmds_found) {
            ACYCLIC_PLAT_DBG_PRINTF("[ac]     changing to sub command list");
            cmd = cmds_found->sub;
        }
    }

    /* show available commands on double tab */
    acyclic_cmd_show(a, cmd, 1);
}


/*****************************************************************************/
/** Backspace key handler
 */
static void acyclic_bs(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    if (a->cmdline_len) {
        a->cmdline_len--;
        ACYCLIC_PLAT_BACKSPACE();
    }
    ACYCLIC_PLAT_DBG_PRINTF("[back]   a->cmdline_len: %3u | a->cmdline: '%.*s'", a->cmdline_len, a->cmdline_len, a->cmdline);
}


/*****************************************************************************/
/** Show AC targets
 */
static void acyclic_cmd_show(
    ACYCLIC_T *a,                               /**< instance handle */
    ACYCLIC_CMD_T *cmd,                         /**< commands */
    uint8_t flg_show_all                        /**< show all flag */
)
{
    if (2 > a->cnt_tab) {
        return;
    }

    if (cmd) {
        ACYCLIC_PLAT_NEWLINE();

        for (; cmd; cmd = (flg_show_all) ? cmd->next : cmd->next_ac) {
            ACYCLIC_PLAT_PUTS(cmd->name);
            ACYCLIC_PLAT_SPACE();
        }
    }

    ACYCLIC_PLAT_NEWLINE();
    a->flg_prompt = 1;
    a->flg_cmd_show = 1;
}


/*****************************************************************************/
/** Skip spaces
 *
 * Skip spaces until string end or next non-space character.
 *
 * @retval len
 */
static void acyclic_space_skip(
    char **str,                                 /**< parser string */
    unsigned int *len                           /**< string length */
)
{
    /* eat preceding spaces */
    for (; *len && (' ' == **str); (*str)++, (*len)--) {
        ACYCLIC_PLAT_DBG_PRINTF("[ac]     len: %u | pre-space skipped: '%.*s'", *len, *len, *str);
    }
    ACYCLIC_PLAT_DBG_PRINTF("[ac]     len: %u | pre-space check finished: '%.*s'", *len, *len, *str);
}


/*****************************************************************************/
/** Return substring and type
 *
 * @retval ACYCLIC_SUBSTR_OPEN
 * @retval ACYCLIC_SUBSTR_CLOSED
 */
static uint8_t acyclic_substr(
    ACYCLIC_T *a,                               /**< instance handle */
    char *sub,                                  /**< substring */
    unsigned int *sub_len,                      /**< substring length */
    unsigned int *len                           /**< overall length */
)
{
    uint8_t res = ACYCLIC_SUBSTR_OPEN;

    for (*sub_len = 0; *sub_len < *len; (*sub_len)++) {
        if (sub[*sub_len] == '"') {
            a->flg_string = !a->flg_string;
        }

        if (sub[*sub_len] == ' ') {
            if (!a->flg_string) {
                res = ACYCLIC_SUBSTR_CLOSED;
                break;
            }
        }
    }

    *len -= *sub_len;

    ACYCLIC_PLAT_DBG_PRINTF("[substr] str: '%.*s', len: %d", *sub_len, sub, *sub_len);
    return res;
}


/*****************************************************************************/
/** Add character to input string
 */
static void acyclic_char_add(
    ACYCLIC_T *a,                               /**< instance handle */
    char chr                                    /**< character */
)
{
#if ACYCLIC_HISTORY == 1
    unsigned int hist_len;                      /* history element len */
#endif

    if (a->cmdline_len >= ACYCLIC_CMDLINE_LEN) {
        return;
    }

#if ACYCLIC_HISTORY == 1
    /* check if oldest history element needs to be overwritten
     * note: "+ 1" for at least one movebyte */
    if (a->hist_cnt && ((a->cmdline + a->cmdline_len + sizeof(a->cmdline_len) + 1) >= a->hist)) {

        /* copy element len with care of alignment */
        a->hist_cnt--;
        memmove(&hist_len, a->hist + a->hist_cnt * sizeof(a->cmdline_len), sizeof(a->cmdline_len));

        /* remove oldest history element */
        memmove(a->hist + sizeof(a->cmdline_len) + hist_len, a->hist, a->hist_cnt * sizeof(a->cmdline_len));
        a->hist += sizeof(a->cmdline_len) + hist_len;
    }
#endif /* ACYCLIC_HISTORY == 1 */

    a->cmdline[a->cmdline_len++] = chr;
    ACYCLIC_PLAT_PUTC(chr);
    ACYCLIC_PLAT_DBG_PRINTF("[add]    a->cmdline_len: %3u | a->cmdline[%u]: %c | a->cmdline: '%.*s'", a->cmdline_len, a->cmdline_len - 1, chr, a->cmdline_len, a->cmdline);
}


/*****************************************************************************/
/** Handle non-control keys
 */
static void acyclic_key(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    if (ACYCLIC_IS_KEY(a->key)) {
        acyclic_char_add(a, (char) a->key);
    }
    else {
        ACYCLIC_PLAT_DBG_PRINTF("[undef]  a->key: %u", a->key);
    }
}


/*****************************************************************************/
/** Handle enter key
 */
static void acyclic_enter(
    ACYCLIC_T *a                                /**< instance handle */
)
{
#if ACYCLIC_DBG_ARGS_SHOW == 1
    uint8_t cnt_arg;                            /* argument counter */
    uint8_t pos_arg;                            /* argument string position */
#endif

    ACYCLIC_PLAT_NEWLINE();

    /* reset application arguments */
    a->arg_cnt = 0;
    a->args[0].cmd = NULL;
    a->func = NULL;

    acyclic_ac(a);
    if (acyclic_flg_exit) {
        return;
    }
    a->flg_prompt = 1;
    ACYCLIC_PLAT_DBG_PRINTF("[enter]  a->cmdline_len: %3u", a->cmdline_len);

    /* check if strings are closed */
    if (a->flg_string) {
        a->flg_string = 0;
        ACYCLIC_PLAT_PUTS_NL("incomplete string found");
        return;
    }

#if ACYCLIC_DBG_ARGS_SHOW == 1
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
#endif /* ACYCLIC_DBG_ARGS_SHOW == 1 */

    /* call function callback if available */
    if (a->args[0].cmd) {
        if (a->func) {
            a->res_func = a->func(a);
        } else {
            ACYCLIC_PLAT_PUTS_NL("no function assigned");
        }
    }

#if ACYCLIC_HISTORY == 1
    /* move command to history */
    acyclic_history_push(a);
#endif

    /* clear command */
    a->cmdline_len = 0;
}


/*****************************************************************************/
/** Print prompt and commandline
 */
static void acyclic_cmdline(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    unsigned int cnt;

    /* show prompt */
    if (a->flg_prompt) {
        ACYCLIC_PLAT_PUTS(ACYCLIC_PROMPT);
        a->flg_prompt = 0;
    }

    if (a->flg_cmd_show) {
        for (cnt = 0; cnt < a->cmdline_len; cnt++) {
            ACYCLIC_PLAT_PUTC(a->cmdline[cnt]);
        }
        a->flg_cmd_show = 0;
    }
}


/*****************************************************************************/
/** Compare two strings
 *
 * @retval ACYCLIC_CMP_MATCH strings match
 * @retval ACYCLIC_CMP_MISMATCH strings mismatch
 * @retval ACYCLIC_CMP_END_FIRST first string ended first
 * @retval ACYCLIC_CMP_END_SECOND second string ended first
 */
static uint8_t acyclic_cmp(
    const char *first,                          /**< first string */
    const char *second,                         /**< second string */
    unsigned int len_first,                     /**< first length */
    unsigned int len_second                     /**< second length */
)
{
    uint8_t flg_end_first = 0;
    uint8_t flg_end_second = 0;
    unsigned int cnt;

    for (cnt = 0; 1; cnt++) {

        if (cnt >= len_first) {
            flg_end_first = 1;
        }

        if (cnt >= len_second) {
            flg_end_second = 1;
        }

        if (flg_end_first || flg_end_second) {
            break;
        }

        if (first[cnt] != second[cnt]) {
            return ACYCLIC_CMP_MISMATCH;
        }
    }

    if (flg_end_first && flg_end_second) {
        return ACYCLIC_CMP_MATCH;
    }

    if (flg_end_first) {
        return ACYCLIC_CMP_END_FIRST;
    }

    /* end of destination string */
    return ACYCLIC_CMP_END_SECOND;
}


/*****************************************************************************/
/** Handle meta keys
 *
 * @retval 0 normal key received
 * @retval 1 meta key processed
 */
static int acyclic_key_pre(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    /* detect and skip UTF-8 multibyte sequences (http://stackoverflow.com/a/1319229) */
    if (a->cnt_utf8) {
        a->cnt_utf8--;
        return 1;
    }

    /* detect UTF-8 intro sequence */
    while (ACYCLIC_KEY_UTF8_1B <= a->key) {

        if ((ACYCLIC_KEY_UTF8_4B_BEG <= a->key) && (ACYCLIC_KEY_UTF8_4B_END >= a->key)) {
            a->cnt_utf8 = ACYCLIC_KEY_UTF8_BYTES_4;
        }
        else if (ACYCLIC_KEY_UTF8_3B <= a->key) {
            a->cnt_utf8 = ACYCLIC_KEY_UTF8_BYTES_3;
        }
        else if (ACYCLIC_KEY_UTF8_2B <= a->key) {
            a->cnt_utf8 = ACYCLIC_KEY_UTF8_BYTES_2;
        }
        else {
            break;
        }

        return 1;
    }

    /* detect escape sequence start */
    if (!a->flg_key_escape) {
        if (ACYCLIC_KEY_ESCAPE == a->key) {
            a->flg_key_escape = 1;
            return 1;
        }

        return 0;
    }

    /* detect left bracket as second escape sequence char */
    if (!a->flg_key_cursor) {
        if (ACYCLIC_KEY_LEFT_BRACKET == a->key) {
            a->flg_key_cursor = 1;
            return 1;
        }

        return 0;
    }
#if ACYCLIC_HISTORY == 1
    else {
        switch (a->key) {
            case ACYCLIC_KEY_UP:
                acyclic_history_up(a);
                break;

            case ACYCLIC_KEY_DOWN:
                acyclic_history_down(a);
                break;
        }
    }
#endif

    /* clear escape sequence state */
    a->flg_key_cursor = 0;
    a->flg_key_escape = 0;

    return 1;
}


#if ACYCLIC_HISTORY == 1
/*****************************************************************************/
/** Store current command in history
 *
 * History structure: | Len1 | Len2 | Len3 | Cmd3 | Cmd2 | Cmd1 |
 * Entry 1 is the newest and 3 the oldest.
 */
static void acyclic_history_push(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    unsigned int hist_len;                      /* history content len */
    unsigned int mov_len;                       /* move space len */

    /* check if cmdline is empty or if there is no space for cmdline + length +
     * minimal move space */
    if ((!a->cmdline_len) ||
        (a->cmdline_len >= (ACYCLIC_CMDLINE_LEN - sizeof(a->cmdline_len) - 1))) {
        return;
    }

    /* add new length to history length storage, memmove is used to don't run
     * into alignment problems */
    a->hist -= sizeof(a->cmdline_len);
    memmove(a->hist, &a->cmdline_len, sizeof(a->cmdline_len));

    /* calculate history length */
    hist_len = (unsigned int) (a->cmdline + ACYCLIC_CMDLINE_LEN - a->hist);
    mov_len = ACYCLIC_CMDLINE_LEN - hist_len - a->cmdline_len;

    while (a->cmdline_len) {
        if (a->cmdline_len < mov_len) {
            mov_len = a->cmdline_len;
        }

        /* move history in move space direction */
        a->hist -= mov_len;
        memmove(a->hist, &a->hist[mov_len], hist_len);

        /* move command to history end position */
        a->cmdline_len -= mov_len;
        memmove(&a->hist[hist_len], &a->cmdline[a->cmdline_len], mov_len);
    }

    /* increase history counter */
    a->hist_cnt++;
}


/*****************************************************************************/
/** Restore selected history command
 *
 * History structure: | Len1 | Len2 | Len3 | Cmd3 | Cmd2 | Cmd1 |
 * Entry 1 is the newest and 3 the oldest.
 */
static void acyclic_history_pop(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    unsigned int mov_len;                       /* move space len */
    char *hist_entry;                           /* history entry */

    /* clear commandline */
    a->cmdline_len = 0;

    /* remove specific length from length array */
    memmove(&a->hist[(a->hist_cnt - a->hist_scroll_cnt + 1) * sizeof(a->cmdline_len)], a->hist, (a->hist_scroll_cnt - 1) * sizeof(a->cmdline_len));
    a->hist_cnt--;
    a->hist += sizeof(a->cmdline_len);

    /* calculate lengths and positions */
    mov_len = (unsigned int) (a->hist - a->cmdline);
    hist_entry = a->cmdline + ACYCLIC_CMDLINE_LEN - a->hist_scroll_idx;

    /* move selected commandline to command */
    while (a->hist_scroll_len) {

        if (a->hist_scroll_len < mov_len) {
            mov_len = a->hist_scroll_len;
        }

        /* move history entry to commandline */
        memmove(&a->cmdline[a->cmdline_len], hist_entry, mov_len);

        /* shrink history entry */
        memmove(&a->hist[mov_len], a->hist, (unsigned int) (hist_entry - a->hist));

        hist_entry += mov_len;
        a->cmdline_len += mov_len;
        a->hist += mov_len;
        a->hist_scroll_len -= mov_len;
    }

    /* end scrolling */
    a->hist_scroll_cnt = 0;
    a->hist_scroll_idx = 0;
}


/*****************************************************************************/
/** Scroll to older entries in history
 */
static void acyclic_history_up(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    char *hist_entry;
    unsigned int cnt;

    /* stop at history end */
    if (a->hist_scroll_cnt >= a->hist_cnt) {
        return;
    }

    /* first scroll => backup commandline */
    if (!a->hist_scroll_cnt) {
        a->hist_scroll_len = a->cmdline_len;
    }

    /* remove current line */
    for (cnt = 0; cnt < a->hist_scroll_len; cnt++) {
        ACYCLIC_PLAT_BACKSPACE();
    }

    /* read current entry length without alignment struggle */
    memmove(&a->hist_scroll_len, &a->hist[a->hist_scroll_cnt * sizeof(a->cmdline_len)], sizeof(a->cmdline_len));
    a->hist_scroll_idx += a->hist_scroll_len;

    /* print history line */
    hist_entry = &a->cmdline[ACYCLIC_CMDLINE_LEN - a->hist_scroll_idx];
    for (cnt = 0; cnt < a->hist_scroll_len; cnt++) {
        ACYCLIC_PLAT_PUTC(hist_entry[cnt]);
    }

    /* scroll up */
    a->hist_scroll_cnt++;
}


/*****************************************************************************/
/** Scroll to newer entries in history
 */
static void acyclic_history_down(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    char *hist_entry;
    unsigned int cnt;

    /* stop at history start */
    if (!a->hist_scroll_cnt) {
        return;
    }

    /* remove current line */
    for (cnt = 0; cnt < a->hist_scroll_len; cnt++) {
        ACYCLIC_PLAT_BACKSPACE();
    }

    /* scroll down */
    a->hist_scroll_cnt--;

    /* restore commandline on last element */
    if (!a->hist_scroll_cnt) {
        a->hist_scroll_idx = 0;
        a->flg_cmd_show = 1;
        acyclic_cmdline(a);
        return;
    }

    /* step over current element */
    a->hist_scroll_idx -= a->hist_scroll_len;

    /* read current entry length without alignment struggle */
    memmove(&a->hist_scroll_len, &a->hist[(a->hist_scroll_cnt - 1) * sizeof(a->cmdline_len)], sizeof(a->cmdline_len));

    /* print history line */
    hist_entry = &a->cmdline[ACYCLIC_CMDLINE_LEN - a->hist_scroll_idx];
    for (cnt = 0; cnt < a->hist_scroll_len; cnt++) {
        ACYCLIC_PLAT_PUTC(hist_entry[cnt]);
    }
}


#  if ACYCLIC_HISTORY_DUMP == 1
/*****************************************************************************/
/** Dump history
 */
static void acyclic_history_dump(
    ACYCLIC_T *a                                /**< instance handle */
)
{
    unsigned int cnt;                           /* counter */
    unsigned int len;                           /* length */
    char *pos;                                  /* position */
    unsigned int hist_len;                      /* history length */

    printf("\n|");
    for (cnt = 0; cnt < a->cmdline_len; cnt++) {
        printf("%c", a->cmdline[cnt]);
    }
    printf("|");

    len = (unsigned int) (a->hist - a->cmdline - a->cmdline_len);
    for (cnt = 0; cnt < len; cnt++) {
        printf("P");
    }
    printf("|");

    for (cnt = 0; cnt < a->hist_cnt; cnt++) {
        memmove(&hist_len, &a->hist[cnt * sizeof(a->cmdline_len)], sizeof(a->cmdline_len));
        printf("%u_%u|", cnt, hist_len);
    }

    pos = a->hist + a->hist_cnt * sizeof(a->cmdline_len);;
    for (cnt = a->hist_cnt; cnt; cnt--) {
        memmove(&hist_len, &a->hist[(cnt - 1) * sizeof(a->cmdline_len)], sizeof(a->cmdline_len));
        printf("%u_%.*s|", cnt - 1, hist_len, pos);
        pos += hist_len;
    }
    printf("\n");

    a->flg_cmd_show = 1;
    a->flg_prompt = 1;
}
#  endif /* ACYCLIC_HISTORY_DUMP == 1 */
#endif /* ACYCLIC_HISTORY == 1 */


/*****************************************************************************/
/** Add command to command list
 */
int acyclic_cmd_add(
    ACYCLIC_T *a,
    ACYCLIC_CMD_T **cmd_root,                   /**< root */
    const char * const cmd_name,                /**< name */
    ACYCLIC_CMD_FUNC_T cmd_func,                /**< function */
    ACYCLIC_CMD_T **cmd_new                     /**< new command */
)
{
    struct ACYCLIC_CMD_T **cmd;

    ACYCLIC_UNUSED(a);

    /* find end of command list */
    for (cmd = cmd_root; *cmd; cmd = &(*cmd)->next);

    /* allocate element for new command */
    *cmd = ACYCLIC_PLAT_CALLOC(sizeof(ACYCLIC_CMD_T));
    if (!*cmd) {
        return -1;
    }

    /* assign command name */
    (*cmd)->name = cmd_name;
    (*cmd)->len = (unsigned int) strlen(cmd_name);
    (*cmd)->func = cmd_func;

    /* assign new command to callers variable */
    if (cmd_new) {
        *cmd_new = *cmd;
    }

    return 0;
}
