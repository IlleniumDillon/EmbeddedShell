/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 12:57:17
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-16 18:40:41
 * @FilePath: \EmbeddedShell\include\eShell.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ESHELL_H_
#define _ESHELL_H_

#include "eShell_type.h"

#define shell_start()           \
    {                           \
        ansi_init(&ansi);       \
        shell_init(&sh);        \
    }                   

#define shell_run()             \
    {                           \
        if(ansi_get_char(ansiPort_getChar(), &ansi) == SHELL_END_CHAR)  \
        {                       \
            shell_parser(&sh, ansi.lineBuf);   \
            ansi_clear_current_line(&ansi);         \
        }                       \
    }

extern shell sh;

void shell_init(shell *sh);

void shell_parser(shell *sh, char *str);

char *shell_cmd_complete(shell *sh, char *str);

void shellHist_queueInit(shellHistQueue *queue);

void shellHist_queueAddCmd(shellHistQueue *queue, char *str);

uint16_t shellHist_queueSearchCmd(shellHistQueue *queue, char *str);

void shellHist_copyQueueItem(shellHistQueue *queue, uint16_t i, char *str_buf);

#define SHELL_USED  __root

#define SHELL_SECTION(x)    __attribute__((section(".rodata.shell_cmd" x)))

#define SHELL_CMD_EXPORT_START(cmd, func) \
    SHELL_USED const cmdFunction _cmd_start_ SHELL_SECTION("0.end") = {#cmd, NULL}

#define SHELL_CMD_EXPORT(cmd, func) \
    SHELL_USED const cmdFunction _cmd_##cmd SHELL_SECTION("1") = {#cmd, func}

#define SHELL_CMD_EXPORT_END(cmd, func) \
    SHELL_USED const cmdFunction _cmd_end_ SHELL_SECTION("1.end") = {#cmd, NULL}

extern const cmdFunction _cmd_start_;

#define _cmd_start_add (&_cmd_start_+1)

#endif