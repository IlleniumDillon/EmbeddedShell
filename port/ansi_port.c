/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 14:04:06
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-16 11:40:03
 * @FilePath: \EmbeddedShell\port\ansi_port.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ansi.h"
#include "eShell.h"
#include <stdarg.h>

// 输入和输出
void ansiPort_init(void)
{

}

void ansiPort_putChar(char x)
{
    printf("%c",x);
}

char ansiPort_getChar(void)
{
    return getchar();
}

void ansi_putStr(char *str, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
    {
        ansiPort_putChar(str[i]);
    }
}

void ansiPort_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    uint8_t len = 0;
    char str[255] = {0};
    len  += vsnprintf(str, 255, format, args);
    va_end(args);
    ansi_putStr(str,len);
}

void ansi_ctrl_common_slover(ansiBuf *ansi)
{
    for(uint8_t i = 0; i < ansi->cmdNum; i++)
    {
        shell_showChar(ansi->cmdBuf[i]);
    }
}
void ansi_in_newline(ansiBuf *ansi)
{
    ansi->courserPos = -1;
    ansi->counter = 0;
    sh.cmdHist.indx = sh.cmdHist.len;

#if SHELL_END_OF_LINE_TYPE != 1
    shell_showChar('\r');
    shell_showChar('\n');
#else
    shell_showChar('\n');
#endif
}
void ansi_in_backspace(ansiBuf *ansi)
{
    if (ansi->courserPos >= 0)
    {
        for (int i = ansi->courserPos; i < ansi->counter; i++)
        {
            ansi->lineBuf[i] = ansi->lineBuf[i + 1];
        }

        ansi->courserPos--;
        ansi->counter--;

        shell_showChar('\b');
        shell_printf(ANSI_CLR_R_MV_L_NCHAR(1));
    }
}
void ansi_in_up(ansiBuf *ansi)
{
    if (sh.cmdHist.indx > 0)
    {
        shell_printf("\033[%dD", ansi->courserPos + 1);
        shell_printf(ANSI_CLEAR_RIGHT);

        shellHist_copyQueueItem(&sh.cmdHist, sh.cmdHist.indx, ansi->lineBuf);
        ansi->counter = strlen(ansi->lineBuf);
        ansi->courserPos = ansi->counter - 1;

        ansi_putStr(ansi->lineBuf, ansi->counter);
		
		sh.cmdHist.indx--;
        sh.cmdHist.indx = (sh.cmdHist.indx == 0) ? sh.cmdHist.len : sh.cmdHist.indx;
    }
}
void ansi_in_down(ansiBuf *ansi)
{
    if (sh.cmdHist.indx > 0)
    {
        shell_printf("\033[%dD", ansi->courserPos + 1);
        shell_printf(ANSI_CLEAR_RIGHT);

        shellHist_copyQueueItem(&sh.cmdHist, sh.cmdHist.indx, ansi->lineBuf);
        ansi->counter = strlen(ansi->lineBuf);
        ansi->courserPos = ansi->counter - 1;

        ansi_putStr(ansi->lineBuf, ansi->counter);
		
		sh.cmdHist.indx++;
        sh.cmdHist.indx = (sh.cmdHist.indx > sh.cmdHist.len) ? 1 : sh.cmdHist.indx;
    }
}
void ansi_in_left(ansiBuf *ansi)
{
    if (ansi->courserPos > -1)
    {
        ansi->courserPos--;
        shell_printf("\033[1D");
    }
}
void ansi_in_right(ansiBuf *ansi)
{
    if (ansi->courserPos < (int)(ansi->counter - 1))
    {
        ansi->courserPos++;
        shell_printf("\033[1C");
    }
}
void ansi_in_tab(ansiBuf *ansi)
{
    char *cmd = shell_cmd_complete(&sh, ansi->lineBuf);
    if(cmd!=NULL)
    {
        if (ansi->counter == 0)
        {
            shell_printf("\r\n");
            for (uint8_t i = 0; sh.cmd[i].fp != NULL; i++)
            {
                shell_printf("%s",sh.cmd[i].cmd);
                shell_printf("\r\n");
            }

            shell_printf("%s",sh.user_name);
        }
        else
        {
            shell_printf("\033[%dD", ansi->courserPos + 1);
            shell_printf(ANSI_CLEAR_RIGHT);
            ansi->counter = strlen(cmd);
            ansi->courserPos= ansi->counter - 1;
            strcpy(ansi->lineBuf, cmd);

            ansi_putStr(ansi->lineBuf, ansi->counter);
        }
    }
}
void ansi_in_enter(ansiBuf *ansi)
{
#if SHELL_END_OF_LINE_TYPE == 1
	ansi->courserPos = -1;
    ansi->counter = 0;
    sh.cmdHist.indx = sh.cmdHist.len;

    shell_showChar('\r');
    shell_showChar('\n');
#else
    shell_showChar('\r');
#endif
}
void ansi_in__(ansiBuf *ansi)
{
	if(ansi->cmdBuf[2] == '3')
	{
		for(int i = ansi->courserPos+1;i<ansi->counter;i++)
		{
			ansi->lineBuf[i] = ansi->lineBuf[i+1];
		}
		if((short)ansi->counter > ansi->courserPos)
		{
			ansi->counter--;
			shell_printf("\033[1P");
		}

	}
}
void ansi_common_char_slover(ansiBuf *ansi, char x)
{
    if(ansi->counter < ANSI_LINE_SIZE - 2)
    {
        if(ansi->courserPos < ansi->counter && ansi->courserPos!=-1)
        {
            for(uint8_t i = ansi->counter; i > ansi->courserPos; i--)
            {
                ansi->lineBuf[i] = ansi->lineBuf[i - 1];
            }
        }
        ansi->courserPos++;
        ansi->counter++;
        ansi->lineBuf[ansi->courserPos] = x;
        ansi->lineBuf[ansi->counter] = '\0';

        if(ansi->courserPos+1 < ansi->counter)
        {
            shell_printf("\033[1@");
        }
        shell_showChar(x);
    }
    else
    {
        ansi->counter = ANSI_LINE_SIZE - 3;
        if (ansi->courserPos >= ansi->counter)
        {
            ansi->courserPos = ansi->counter - 1;
        }
        ansi->lineBuf[ansi->counter] = '\0';
    }
}