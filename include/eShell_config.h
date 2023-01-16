/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 13:00:00
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-15 22:10:17
 * @FilePath: \EmbeddedShell\include\eShell_config.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ESHELL_CONFIG_H_
#define _ESHELL_CONFIG_H_

// 每条命令字符数
#define SHELL_CMD_LINE_MAX_LENGTH ANSI_LINE_SIZE

// 用户名长度
#define SHELL_USER_NAME_MAX_LENGTH (10)

// 命令字符数
#define SHELL_CMD_NAME_MAX_LENGTH (10)

// 命令最大参数个数
#define SHELL_CMD_PARAS_MAX_NUM (10)

// 保存历史记录个数
#define SHELL_MAX_CMD_HISTORY_NUM (3)

// 历史空间大小
#define SHELL_CMD_HISTORY_BUF_LENGTH (253)

// 用户名
#define SHELL_USER_NAME "Shell:>"

// 换行符
#define SHELL_END_OF_LINE_TYPE (2)
/*
    0   "\n"
    1   "\r"
    2   "\r\n"
*/
#if SHELL_END_OF_LINE_TYPE == 0
#define SHELL_END_OF_LINE "\n"
#elif SHELL_END_OF_LINE_TYPE == 1
#define SHELL_END_OF_LINE "\r"
#elif SHELL_END_OF_LINE_TYPE == 2
#define SHELL_END_OF_LINE "\r\n"
#else
#define SHELL_END_OF_LINE "\r\n"
#endif

//#define SHELL_END_OF_LINE "\r\n"

#define shell_showChar(x) ansiPort_putChar(x)
#define shell_printf(fmt,...) ansiPort_printf(fmt,##__VA_ARGS__)

#endif