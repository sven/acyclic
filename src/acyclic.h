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
#ifndef ACYCLIC_H
#define ACYCLIC_H


/*****************************************************************************/
/* Configuration */
/*****************************************************************************/
#define ACYCLIC_DBG                 0
#define ACYCLIC_DBG_ARGS_SHOW       0
#define ACYCLIC_HISTORY             1
#define ACYCLIC_HISTORY_DUMP        0


/*****************************************************************************/
/* Forward declarations */
/*****************************************************************************/
struct ACYCLIC_T;


/*****************************************************************************/
/* Include platform handling */
/*****************************************************************************/
#include <acyclic_platform.h>


/*****************************************************************************/
/* Defines */
/*****************************************************************************/
#define ACYCLIC_UNUSED(x)           (void)(x)

#define ACYCLIC_CMDLINE_LEN         50
#define ACYCLIC_ARG_COUNT           5
#define ACYCLIC_PROMPT              "$ "

#define ACYCLIC_CMP_MATCH           0
#define ACYCLIC_CMP_MISMATCH        1
#define ACYCLIC_CMP_END_FIRST       2
#define ACYCLIC_CMP_END_SECOND      3

#define ACYCLIC_SRCH_NONE           0
#define ACYCLIC_SRCH_FOUND          1
#define ACYCLIC_SRCH_AC             2

#define ACYCLIC_SUBSTR_OPEN         0
#define ACYCLIC_SUBSTR_CLOSED       1

#define ACYCLIC_KEY_TAB             9
#define ACYCLIC_KEY_ESCAPE          0x1b
#define ACYCLIC_KEY_LEFT_BRACKET    0x5b
#define ACYCLIC_KEY_UP              0x41
#define ACYCLIC_KEY_DOWN            0x42
#define ACYCLIC_KEY_RIGHT           0x43
#define ACYCLIC_KEY_LEFT            0x44
#define ACYCLIC_IS_KEY(key)         ((key >= 0x20) && (key <= 0x7e)) || ((key >= 0x80))
#define ACYCLIC_KEY_UTF8_1B         0x80
#define ACYCLIC_KEY_UTF8_2B         0xc0
#define ACYCLIC_KEY_UTF8_3B         0xe0
#define ACYCLIC_KEY_UTF8_4B_BEG     0xf0
#define ACYCLIC_KEY_UTF8_4B_END     0xf4
#define ACYCLIC_KEY_UTF8_BYTES_2    1
#define ACYCLIC_KEY_UTF8_BYTES_3    2
#define ACYCLIC_KEY_UTF8_BYTES_4    3

#define ACYCLIC_FALSE               0
#define ACYCLIC_TRUE                1


/*****************************************************************************/
/* Forward declarations */
/*****************************************************************************/
struct ACYCLIC_CMD_DATA_T;


/*****************************************************************************/
/* Typedefs */
/*****************************************************************************/
typedef uint8_t (* ACYCLIC_CMD_FUNC_T)(struct ACYCLIC_T *a);


/*****************************************************************************/
/* Structures */
/*****************************************************************************/

/**< ACyCLIC command data */
typedef struct ACYCLIC_CMD_T {
    const char * name;                          /**< command name */
    unsigned int len;                           /**< command length */

    struct ACYCLIC_CMD_T *next;                 /**< next command */
    struct ACYCLIC_CMD_T *sub;                  /**< sub commands */

    struct ACYCLIC_CMD_T *next_ac;              /**< next found AC command */
} ACYCLIC_CMD_T;


/**< ACyCLIC root command data, like ACYCLIC_CMD_T but with function pointer */
typedef struct ACYCLIC_CMD_ROOT_T {
    const char * name;                          /**< command name */
    unsigned int len;                           /**< command length */

    struct ACYCLIC_CMD_T *next;                 /**< next command */
    struct ACYCLIC_CMD_T *sub;                  /**< sub commands */

    struct ACYCLIC_CMD_T *next_ac;              /**< next found AC command */

    ACYCLIC_CMD_FUNC_T func;                    /**< command function */
} ACYCLIC_CMD_ROOT_T;


/**< ACyCLIC arguments data */
typedef struct ACYCLIC_ARG_T {
    const char *name;                           /**< argument name start */
    unsigned int len;                           /**< argument name length */
    ACYCLIC_CMD_T *cmd;                         /**< command pointer */
} ACYCLIC_ARG_T;


/**< ACyCLIC instance data */
typedef struct ACYCLIC_T {
    ACYCLIC_CMD_T *cmds;                        /**< root command list */

    char cmdline[ACYCLIC_CMDLINE_LEN];          /**< commandline */
    unsigned int cmdline_len;                   /**< commandline length */

#if ACYCLIC_HISTORY == 1
    char *hist;                                 /**< history start pointer */
    unsigned int hist_cnt;                      /**< history element counter */
    unsigned int hist_scroll_cnt;               /**< history scroll counter */
    unsigned int hist_scroll_idx;               /**< history scroll element index */
    unsigned int hist_scroll_len;               /**< length of previous element */
#endif

    /* application arguments */
    uint8_t arg_cnt;                            /**< count of found arguments */
    ACYCLIC_ARG_T args[ACYCLIC_ARG_COUNT];      /**< found arguments */

    uint8_t key;                                /**< current key */
    uint8_t cnt_tab;                            /**< tab key count */

    uint8_t flg_string;                         /**< in-string flag */
    uint8_t flg_prompt;                         /**< show prompt */
    uint8_t flg_cmd_show;                       /**< show commandline */
    uint8_t flg_match_exact;                    /**< exact match flag */
    uint8_t flg_key_escape;                     /**< escape sequence flag */
    uint8_t flg_key_cursor;                     /**< cursor key flag */

    uint8_t cnt_utf8;                           /**< UTF-8 char skip counter */
    uint8_t res_func;                           /**< result of last called function */
} ACYCLIC_T;


/*****************************************************************************/
/* Global variables */
/*****************************************************************************/
extern uint8_t acyclic_flg_exit;                /**< exit flag */


/*****************************************************************************/
/* Prototypes */
/*****************************************************************************/
int acyclic_init(
    ACYCLIC_T *a                                /**< ACyCLIC handle */
);

void acyclic_input(
    ACYCLIC_T *a,                               /**< instance handle */
    uint8_t key                                 /**< input */
);

#ifndef ACYCLIC_CNF_CMD_NOT_FOUND
#  define acyclic_cmd_not_found(x)  1
#else
uint8_t acyclic_cmd_not_found(
    ACYCLIC_T *a
);
#endif

int acyclic_cmd_add(
    ACYCLIC_T *a,
    ACYCLIC_CMD_T **cmd_root,                   /**< root */
    const char * const cmd_name,                /**< name */
    ACYCLIC_CMD_FUNC_T cmd_func,                /**< function */
    ACYCLIC_CMD_T **cmd_new                     /**< new command */
);

int acyclic_cmd_reg(
    struct ACYCLIC_T *a
);


#endif /* ACYCLIC_H */
