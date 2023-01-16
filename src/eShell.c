/*
 * @Author: IlleniumDillon 147900130@qq.com
 * @Date: 2023-01-15 12:57:29
 * @LastEditors: IlleniumDillon 147900130@qq.com
 * @LastEditTime: 2023-01-16 11:41:37
 * @FilePath: \EmbeddedShell\src\eShell.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "eShell.h"

shell sh = 
{
    .user_name = SHELL_USER_NAME,
};

static char *shell_strtok(char *string_org, const char *demial)
{
	static unsigned char *last;
	unsigned char *str;
	const unsigned char *ctrl = (const unsigned char *)demial;
	unsigned char map[32];
	int count;

	for (count = 0; count < 32; count++)
	{
		map[count] = 0;
	}
	do
	{
		map[*ctrl >> 3] |= (1 << (*ctrl & 7));
	} while (*ctrl++);
	if (string_org)
	{
		str = (unsigned char *)string_org;
	}
	else
	{
		str = last;
	}
	while ((map[*str >> 3] & (1 << (*str & 7))) && *str)
	{
		str++;
	}
	string_org = (char *)str;
	for (; *str; str++)
	{
		if (map[*str >> 3] & (1 << (*str & 7)))
		{
			*str++ = '\0';
			break;
		}
	}
	last = str;
	if (string_org == (char *)str)
	{
		return NULL;
	}
	else
	{
		return string_org;
	}
}

void shell_init(shell* sh)
{
    extern cmdFunction static_cmd[];
    sh->cmd = static_cmd;
    shell_printf("%s",sh->user_name);
	shellHist_queueInit(&sh->cmdHist);
	shellHist_queueAddCmd(&sh->cmdHist, "ls cmd");
	sh->cmdHist.indx = 1;
}

shell_fun_t shell_search_cmd(shell *sh, char *str)
{
	unsigned int i = 0;
	while (sh->cmd[i].fp != NULL)
	{
		if (!strcmp(str, sh->cmd[i].cmd))
		{
			return sh->cmd[i].fp;
		}
		i++;
	}

	return NULL;
}

void shell_parser(shell *sh, char *str)
{
    char argc = 0;
	char argv[SHELL_CMD_LINE_MAX_LENGTH + SHELL_CMD_PARAS_MAX_NUM];
	char *token = str;
	shell_fun_t fp;
	char index = SHELL_CMD_PARAS_MAX_NUM;

    //shell_printf(SHELL_END_OF_LINE);

	if (shellHist_queueSearchCmd(&sh->cmdHist, str) == 0 && str[0] != '\0')
	{
		shellHist_queueAddCmd(&sh->cmdHist, str);
	}

	if (strlen(str) > SHELL_CMD_LINE_MAX_LENGTH)
	{
		shell_printf("this command is too long."SHELL_END_OF_LINE);
		shell_printf("%s",sh->user_name);
		return;
	}

	token = shell_strtok(token, " ");
	fp = shell_search_cmd(sh, str);

	if (fp == NULL)
	{
		if (isalpha(str[0]))
		{
			shell_printf("no command named: %s"SHELL_END_OF_LINE, token);
		}
	}
	else
	{
		argv[argc] = index;
		strcpy(argv + index, str);
		index += strlen(str) + 1;
		argc++;

		token = shell_strtok(NULL, " ");
		while (token != NULL)
		{
			argv[argc] = index;
			strcpy(argv + index, token);
			index += strlen(token) + 1;
			argc++;
			token = shell_strtok(NULL, " ");
		}
	}

	if (fp != NULL)
	{
		fp(argc, argv);
	}

	shell_printf("%s",sh->user_name);
}

char *shell_cmd_complete(shell *sh, char *str)
{
    char *temp = NULL;
	unsigned char i;
	char *best_matched = NULL;
	unsigned char min_position = 255;

	for (i = 0; sh->cmd[i].cmd[0] != '\0'; i++)
	{
		temp = NULL;
		temp = strstr(sh->cmd[i].cmd, str);
		if (temp != NULL && ((unsigned long)temp - (unsigned long)(&sh->cmd[i]) < min_position))
		{
			min_position = (unsigned long)temp - (unsigned long)(&sh->cmd[i]);
			best_matched = (char *)&sh->cmd[i];
			if (min_position == 0)
			{
				break;
			}
		}
	}

	return best_matched;
}

void shellHist_queueInit(shellHistQueue *queue)
{
    queue->fp = 0;
	queue->rp = 0;
	queue->len = 0;

	queue->store_front = 0;
	queue->store_rear = 0;
	queue->store_num = 0;
}

void shellHist_queueAddCmd(shellHistQueue *queue, char *str)
{
    unsigned short int str_len;
	unsigned short int i;

	str_len = strlen(str);

	if (str_len > SHELL_CMD_HISTORY_BUF_LENGTH)
	{
		return;
	}

	while (str_len > (SHELL_CMD_HISTORY_BUF_LENGTH - queue->store_num) || queue->len == SHELL_MAX_CMD_HISTORY_NUM)
	{

		queue->fp++;
		queue->fp = (queue->fp > SHELL_MAX_CMD_HISTORY_NUM) ? 0 : queue->fp;
		queue->len--;

		if (queue->store_front <= queue->queue[queue->fp])
		{
			queue->store_num -= queue->queue[queue->fp] - queue->store_front;
		}
		else
		{
			queue->store_num -= queue->queue[queue->fp] + SHELL_CMD_HISTORY_BUF_LENGTH - queue->store_front + 1;
		}

		queue->store_front = queue->queue[queue->fp];
	}

	queue->queue[queue->rp] = queue->store_rear;
	queue->rp++;
	queue->rp = (queue->rp > SHELL_MAX_CMD_HISTORY_NUM) ? 0 : queue->rp;
	queue->len++;

	for (i = 0; i < str_len; i++)
	{
		queue->buf[queue->store_rear] = str[i];
		queue->store_rear++;
		queue->store_rear = (queue->store_rear > SHELL_CMD_HISTORY_BUF_LENGTH) ? 0 : queue->store_rear;
		queue->store_num++;
	}
	queue->queue[queue->rp] = queue->store_rear;
}

uint16_t shellHist_queueSearchCmd(shellHistQueue *queue, char *str)
{
    unsigned short int str_len;
	unsigned short int i, j;
	unsigned short int index_temp = queue->fp;
	unsigned short int start;
	unsigned short int end;
	unsigned short int cmd_len;
	unsigned short int matched_id = 0;
	unsigned short int buf_index;

	if (queue->len == 0)
	{
		return matched_id;
	}
	else
	{
		str_len = strlen(str);
		for (i = 0; i < queue->len; i++)
		{
			start = queue->queue[index_temp];
			index_temp++;
			index_temp = (index_temp > SHELL_MAX_CMD_HISTORY_NUM) ? 0 : index_temp;
			end = queue->queue[index_temp];

			if (start <= end)
			{
				cmd_len = end - start;
			}
			else
			{
				cmd_len = SHELL_CMD_HISTORY_BUF_LENGTH + 1 - start + end;
			}

			if (cmd_len == str_len)
			{
				matched_id = i + 1;
				buf_index = start;
				for (j = 0; j < str_len; j++)
				{
					if (queue->buf[buf_index] != str[j])
					{
						matched_id = 0;
						break;
					}

					buf_index++;
					buf_index = (buf_index > SHELL_CMD_HISTORY_BUF_LENGTH) ? 0 : buf_index;
				}

				if (matched_id != 0)
				{
					return matched_id;
				}
			}
		}

		return 0;
	}
}

void shellHist_copyQueueItem(shellHistQueue *queue, uint16_t i, char *str_buf)
{
    unsigned short index_temp;
	unsigned short start;
	unsigned short end;
	unsigned short j;

	if (i <= queue->len)
	{
		index_temp = queue->fp + i - 1;
		index_temp = (index_temp > SHELL_MAX_CMD_HISTORY_NUM) ? (index_temp - SHELL_MAX_CMD_HISTORY_NUM - 1) : index_temp;

		start = queue->queue[index_temp];
		index_temp++;
		index_temp = (index_temp > SHELL_MAX_CMD_HISTORY_NUM) ? 0 : index_temp;
		end = queue->queue[index_temp];

		if (start < end)
		{
			for (j = start; j < end; j++)
			{
				str_buf[j - start] = queue->buf[j];
			}

			str_buf[j - start] = '\0';
		}
		else
		{
			for (j = start; j < SHELL_CMD_HISTORY_BUF_LENGTH + 1; j++)
			{
				str_buf[j - start] = queue->buf[j];
			}

			for (j = 0; j < end; j++)
			{
				str_buf[j + SHELL_CMD_HISTORY_BUF_LENGTH + 1 - start] = queue->buf[j];
			}

			str_buf[j + SHELL_CMD_HISTORY_BUF_LENGTH + 1 - start] = '\0';
		}
	}
}