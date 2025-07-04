#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expansion.h"

/**
 * @brief Example integration with shell
 */
int main(int argc, char **argv, char **envp)
{
    t_env *env;
    char input[1024];
    char *expanded;
    int exit_status = 0;

    env = env_init(envp);
    if (!env)
    {
        fprintf(stderr, "Failed to initialize environment\n");
        return 1;
    }

    printf("Current Date and Time: 2025-07-04 04:38:50\n");
    printf("Current User: mtarza13\n");
    
    while (1)
    {
        printf("minishell> ");
        if (!fgets(input, sizeof(input), stdin))
            break;
        
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';
        
        if (strcmp(input, "exit") == 0)
            break;
            
        expanded = expand_variables(env, input, exit_status);
        if (expanded)
        {
            printf("After expansion: [%s]\n", expanded);
            
            /* Here you would:
             * 1. Tokenize the expanded input
             * 2. Parse the tokens
             * 3. Execute the command
             * 4. Update exit_status based on command execution
             */
            
            free(expanded);
        }
    }
    
    env_free(env);
    return 0;
}