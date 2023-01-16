/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 14:01:32
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-15 20:55:46
 * @FilePath: \EmbeddedShell\src\ansi.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ansi.h"

ansiBuf ansi = {0};

const char ansi_in_cmd[] = {'m', 'I', 'A', 'B', 'C', 'D', 'X', 'K', 'M', 'P', 'J', '@', 'L', 'l', 'h', 'n', 'H', 's', 'u', '~','\0'};
void (*const ansi_in_cmd_fun[])(ansiBuf *) =
    {
        ansi_in_m_function,
        ansi_in_I_function,
        ansi_in_A_function,
        ansi_in_B_function,
        ansi_in_C_function,
        ansi_in_D_function,
        ansi_in_X_function,
        ansi_in_K_function,
        ansi_in_M_function,
        ansi_in_P_function,
        ansi_in_J_function,
        ansi_in_at_function,
        ansi_in_L_function,
        ansi_in_l_function,
        ansi_in_h_function,
        ansi_in_n_function,
        ansi_in_H_function,
        ansi_in_s_function,
        ansi_in_u_function,
		ansi_in___function,
	};

const char ansi_in_special_symbol[] = {'\b', '\n', '\r', '\t', '\0'};
void (*const ansi_in_special_symbol_fun[])(ansiBuf *) =
    {
        ansi_in_bsb_function,
        ansi_in_bsn_function,
        ansi_in_bsr_function,
        ansi_in_bst_function};

int8_t ansi_search_char(char x, const char *buf)
{
    int8_t i = 0;
    for (i = 0; (buf[i] != x) && (buf[i] != '\0'); i++);
    if (buf[i] != '\0')
    {
        return i;
    }
    else
    {
        return -1;
    }
}

void ansi_init(ansiBuf* ansi)
{
    extern void ansiPort_init(void);
    ansiPort_init();
    ansi->counter = 0;
    ansi->courserPos = -1;
    ansi->cmdNum = 0;
    ansi->combineState = ANSI_NO_CTRL_CHAR;
}

char ansi_get_char(char x, ansiBuf* ansi)
{
    int8_t cmdId = -1;

    if(ansi->combineState == ANSI_NO_CTRL_CHAR)
    {
        cmdId = ansi_search_char(x, ansi_in_special_symbol);
        if(cmdId >= 0)
        {
            if(ansi_in_special_symbol_fun[cmdId] != NULL)
            {
                ansi_in_special_symbol_fun[cmdId](ansi);
            }
        }
        else if(x == '\033')
        {
            ansi->combineState = ANSI_WAIT_CTRL_END;
            ansi->cmdBuf[ansi->cmdNum] = x;
            ansi->cmdNum++;
        }
        else
        {
            ansi_common_char_slover(ansi, x);
        }
    }
    else if(ansi->combineState == ANSI_WAIT_CTRL_END)
    {
        ansi->cmdBuf[ansi->cmdNum] = x;
        if(('a' <= x && 'z' >= x) || ('A' <= x && 'Z' >= x) || x== '~')
        {
            cmdId = ansi_search_char(x, ansi_in_cmd);
            ansi_in_cmd_fun[cmdId](ansi);
            ansi->cmdNum = 0;
            ansi->combineState = ANSI_NO_CTRL_CHAR;
        }
        else if(ansi->cmdNum > 18)
        {
            ansi->cmdNum = 0;
            ansi->combineState = ANSI_NO_CTRL_CHAR;
        }
        else
        {
            ansi->cmdNum++;
        }
    }
    else
    {
        ansi->combineState = ANSI_NO_CTRL_CHAR;
    }

    return x;
}

void ansi_clear_current_line(ansiBuf* ansi)
{
    ansi->counter = 0;
    ansi->courserPos = -1;
    ansi->lineBuf[ansi->counter] = '\0';
}