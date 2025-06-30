#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <strings.h>
#include "token.h"
#include "parcer.h"

/*
==============================================================================
 main.c - Entry point for minishell lexer test
==============================================================================
*/

void print_cmds(t_cmd *cmd)
{
    int cmd_index = 1;
    while (cmd)
    {
        printf("\n🟢 Command %d:\n", cmd_index++);

        for (int i = 0; i < cmd->argc; i++)
            printf("  argv[%d]: %s\n", i, cmd->argv[i]);

        if (cmd->infile)
            printf("  infile: %s\n", cmd->infile);

        if (cmd->outfile)
            printf("  outfile: %s (append=%d)\n", cmd->outfile, cmd->append);

        cmd = cmd->next;
    }
}

int main()
{
//	char *line = readline("-->");
//	
//	Token *tokens = lexer(line);
//	
//	t_cmd *cmds = parse_tokens(tokens);
//
//	t_cmd *tmp = cmds;
//	while (tmp)
//	{
//	    finalize_args(tmp);
//	    tmp = tmp->next;
//	}
	//	print_cmds(cmds);

	//free_token(tokens);  // Now free the tokens after use
	//free(line);          // Free the readline buffer
	
	return 0;
}
