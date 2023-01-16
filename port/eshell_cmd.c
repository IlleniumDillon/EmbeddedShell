/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 20:29:05
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-16 20:04:48
 * @FilePath: \EmbeddedShell\port\eshell_cmd.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "eShell.h"
#include <stdio.h>

float para = 0;

SHELL_CMD_EXPORT_START(start,NULL);
SHELL_CMD_EXPORT_END(end,NULL);

void shell_ls_cmd(char argc, char *argv)
{
	unsigned int i = 0;
	if (argc > 1)
	{
		if (!strcmp("cmd", &argv[argv[1]]))
		{

			for (i = 0; sh.cmd[i].fp != NULL; i++)
			{
				shell_printf("%s",sh.cmd[i].cmd);
				shell_printf("\r\n");
			}
		}
		else if (!strcmp("-v", &argv[argv[1]]))
		{
			shell_printf("ls version 1.0.\r\n");
		}
		else if (!strcmp("-h", &argv[argv[1]]))
		{
			shell_printf("useage: ls [options]\r\n");
			shell_printf("options: \r\n");
			shell_printf("\t -h \t: show help\r\n");
			shell_printf("\t -v \t: show version\r\n");
			shell_printf("\t cmd \t: show all commands\r\n");
		}
	}
	else
	{
		shell_printf("ls need more arguments!\r\n");
	}
}

/**
 * @brief test command
 */
void shell_test_cmd(char argc, char *argv)
{
	unsigned int i;
	shell_printf("test command:\r\n");
	for (i = 0; i < argc; i++)
	{
		shell_printf("paras %d: %s\r\n", i, &(argv[argv[i]]));
	}
}

void shell_para_op(char argc, char *argv)
{
  if (argc > 1)
  {
    if (!strcmp("-w", &argv[argv[1]]))
    {
        if(!strcmp("-i", &argv[argv[2]]))
        {
          para = atoi(&argv[argv[3]]);
        }
    }
    else if(!strcmp("-r", &argv[argv[1]]))
    {
      shell_printf("parameter value: %f\r\n",para);
    }
    else if(!strcmp("-h", &argv[argv[1]]))
    {
        shell_printf("useage: paraOp [options]\r\n");
        shell_printf("options: \r\n");
        shell_printf("\t -w \t: write parameter\r\n");
        shell_printf("\t -r \t: read parameter\r\n");
        shell_printf("\t -l \t: show all parameters\r\n");
        shell_printf("\t -i \t: integer parameter\r\n");
        shell_printf("\t -fi \t: fix point parameter integer section\r\n");
        shell_printf("\t -fd \t: fix point parameter decimal section\r\n");
    }
    else
    {
      shell_printf("paraOp need more arguments!\r\n");
    }
  }
  else
  {
    shell_printf("paraOp need more arguments!\r\n");
  }
}

SHELL_CMD_EXPORT(ls,shell_ls_cmd);

SHELL_CMD_EXPORT(test,shell_test_cmd);

SHELL_CMD_EXPORT(paraOp,shell_para_op);

/*const cmdFunction static_cmd[] =
	{
		{"ls", shell_ls_cmd},
		{"test", shell_test_cmd},
		{"\0", NULL}
};*/