#include "executor.h"

int get_cmd_count(t_cmd *cmd)
{
	int count = 0;
	while(cmd)
	{
	 count++;
		cmd = cmd->next;
	}
	return count;
}
