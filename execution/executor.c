#include "pipe.h"
#include "child_process.h"
#include "free_memory.h"
#include "wait_status.h"


void execution_pipeline(t_cmd *cmd_list)
{
	create_pipe(cmd_list)
	wait_all_childern();
	free_cmd_data();
}
