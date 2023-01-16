/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 13:05:58
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-16 11:26:05
 * @FilePath: \EmbeddedShell\include\ansi.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _ANSI_H_
#define _ANSI_H_

#include <stdio.h>
#include <stdint.h>
#include "eShell_config.h"

// 背景颜色定义
#define ANSI_BBLACK "40"
#define ANSI_BRED "41"
#define ANSI_BGREEN "42"
#define ANSI_BGRAY "43"
#define ANSI_BBLUE "44"
#define ANSI_BPURPLE "45"
#define ANSI_BAQUAM "46"
#define ANSI_BWHITE "47"

// 字符颜色定义
#define ANSI_FBLACK "30"
#define ANSI_FRED "31"
#define ANSI_FGREEN "32"
#define ANSI_FGRAY "33"
#define ANSI_FBLUE "34"
#define ANSI_FPURPLE "35"
#define ANSI_FAQUAM "36"
#define ANSI_FWHITE "37"

// 字符效果定义
#define ANSI_NORMAL "0"
#define ANSI_BRIGHT "1"
#define ANSI_UNDERLINE "4"
#define ANSI_FLASH "5"
#define ANSI_INVERSE "7"
#define ANSI_INVISABLE "8"

// 字体定义
#define ANSI_NORMAL "0"
#define ANSI_SONG "1"
#define ANSI_HEI "2"
#define ANSI_KAI "3"

// 设定字体字符串
#define ANSI_SET_TEXT(cmd) ((const char *)"\033["##cmd##"m")

// 清除指令
#define ANSI_CLEAR_RIGHT "\033[K"
#define ANSI_CLEAR_LEFT "\033[1K"
#define ANSI_CLEAR_WHOLE "\033[2K"
#define ANSI_CLEAR_SCREEN "\033[2J"

// 光标控制
#define ANSI_HIDE_COURSER "\033[?25l"
#define ANSI_SHOW_COURSER "\033[?25h"
#define ANSI_MV_L_N(n) ((const char *)"\033["#n"D")
#define ANSI_MV_R_N(n) ((const char *)"\033["#n"C")
#define ANSI_SET_FONT(cmd) ((const char *)"\033["#cmd"I")
#define ANSI_CLR_R_NCHAR(cmd) ((const char *)"\033["#cmd"X")
#define ANSI_CLR_R_MV_L_NCHAR(cmd) ((const char *)"\033["#cmd"P")

// 一行有效字符数
#define ANSI_LINE_SIZE (64)
// 控制字符组合长度
#define ANSI_CMD_SIZE (20)

typedef enum _combineState
{
    ANSI_NO_CTRL_CHAR,
    ANSI_WAIT_CTRL_END,
}combineState;

// 字符控制结构体
typedef struct _ansi
{
    int8_t courserPos;             //光标位置
    uint8_t counter;                //字符数
    uint8_t cmdNum;                 //控制字符数
    combineState combineState;      //组合键状态
    char lineBuf[ANSI_LINE_SIZE];   //当前行字符
    char cmdBuf[ANSI_CMD_SIZE];     //控制字符
}ansiBuf;

// 字符操作函数指针定义
typedef void (*ansi_fun_t)(ansiBuf *);

// 字符控制结构体
extern ansiBuf ansi;

/**
 * @description: 初始化字符串控制结构体
 * @param {ansiBuf*} ansi 字符串控制结构体指针
 * @return {*}
 */
void ansi_init(ansiBuf* ansi);

/**
 * @description: 传入一个新的字符
 * @param {char} x 新字符
 * @param {ansiBuf*} ansi 字符串控制结构体指针
 * @return {char}
 */
char ansi_get_char(char x, ansiBuf* ansi);

/**
 * @description: 
 * @param {ansiBuf*} ansi
 * @return {*}
 */
void ansi_clear_current_line(ansiBuf* ansi);


/**
 * @description: 控制字符处理函数
 * @param {ansiBuf*} ansi 字符串控制结构体指针
 * @return {*}
 */
void ansi_ctrl_common_slover(ansiBuf *ansi);
void ansi_in_newline(ansiBuf *ansi);
void ansi_in_backspace(ansiBuf *ansi);
void ansi_in_up(ansiBuf *ansi);
void ansi_in_down(ansiBuf *ansi);
void ansi_in_left(ansiBuf *ansi);
void ansi_in_right(ansiBuf *ansi);
void ansi_in_tab(ansiBuf *ansi);
void ansi_in_enter(ansiBuf *ansi);
void ansi_in__(ansiBuf *ansi);

/**
 * @description: 普通字符处理函数
 * @param {ansiBuf*} ansi 字符串控制结构体指针
 * @param {char} x 字符
 * @return {*}
 */
void ansi_common_char_slover(ansiBuf *ansi, char x);

#define ansi_in_bsb_function ansi_in_backspace
#define ansi_in_bsn_function ansi_in_newline
#define ansi_in_bsr_function ansi_in_enter
#define ansi_in_bst_function ansi_in_tab
#define ansi_in_m_function ansi_ctrl_common_slover
#define ansi_in_I_function ansi_ctrl_common_slover
#define ansi_in_A_function ansi_in_up
#define ansi_in_B_function ansi_in_down
#define ansi_in_C_function ansi_in_right
#define ansi_in_D_function ansi_in_left
#define ansi_in_X_function ansi_ctrl_common_slover
#define ansi_in_K_function ansi_ctrl_common_slover
#define ansi_in_M_function ansi_ctrl_common_slover
#define ansi_in_P_function ansi_ctrl_common_slover
#define ansi_in_J_function ansi_ctrl_common_slover
#define ansi_in_at_function ansi_ctrl_common_slover
#define ansi_in_L_function ansi_ctrl_common_slover
#define ansi_in_l_function ansi_ctrl_common_slover
#define ansi_in_h_function ansi_ctrl_common_slover
#define ansi_in_n_function ansi_ctrl_common_slover
#define ansi_in_H_function ansi_ctrl_common_slover
#define ansi_in_s_function ansi_ctrl_common_slover
#define ansi_in_u_function ansi_ctrl_common_slover
#define ansi_in___function ansi_in__

void ansiPort_init(void);
void ansiPort_putChar(char x);
char ansiPort_getChar(void);
void ansiPort_printf(const char *format, ...);

#endif