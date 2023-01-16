/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 12:59:33
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-15 19:52:02
 * @FilePath: \EmbeddedShell\include\eShell_type.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ESHELL_TYPE_H_
#define _ESHELL_TYPE_H_

#include "eShell_config.h"
#include "ansi.h"

#if SHELL_END_OF_LINE_TYPE == 1
#define SHELL_END_CHAR '\r'
#else
#define SHELL_END_CHAR '\n'
#endif

// Shell 命令
typedef void (*shell_fun_t)(char, char *);

// 命令描述结构体
typedef struct _cmd_function_description
{
    char cmd[SHELL_CMD_NAME_MAX_LENGTH];    //命令名
    void (*fp)(char argc, char *argv);      //功能函数
    char *description;                      //描述信息
}cmdFunction;

typedef struct _shell_history_queue
{
    uint16_t fp,rp,len,indx;
    uint16_t store_front,store_rear,store_num;
    char queue[SHELL_MAX_CMD_HISTORY_NUM + 1];
    char buf[SHELL_CMD_HISTORY_BUF_LENGTH + 1];
}shellHistQueue;

typedef struct _shell
{
    char user_name[SHELL_USER_NAME_MAX_LENGTH];
    const cmdFunction* cmd;
    shellHistQueue cmdHist;
}shell;

#endif