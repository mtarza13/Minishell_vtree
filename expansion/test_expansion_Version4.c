#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expansion.h"

/**
 * @brief Test function for variable expansion
 */
void test_expand(t_env *env, const char *input, int exit_status)
{
    char *expanded = expand_variables(env, input, exit_status);
    
    printf("Input:    [%s]\n", input);
    printf("Expanded: [%s]\n", expanded ? expanded : "NULL");
    printf("----------------------------\n");
    
    if (expanded)
        free(expanded);
}

/**
 * @brief Main test function
 */
int main(int argc, char **argv, char **envp)
{
    t_env *env = env_init(envp);
    
    if (!env)
    {
        fprintf(stderr, "Failed to initialize environment\n");
        return 1;
    }
    
    printf("Current Date and Time (UTC): 2025-07-04 04:38:50\n");
    printf("Current User's Login: mtarza13\n\n");
    
    printf("=== Basic Tests ===\n");
    test_expand(env, "Hello, $USER!", 0);
    test_expand(env, "Path: $PATH", 0);
    test_expand(env, "Exit status: $?", 42);
    
    printf("\n=== Quote Handling Tests ===\n");
    test_expand(env, "Single quotes: '$USER' should not expand", 0);
    test_expand(env, "Double quotes: \"$USER\" should expand", 0);
    test_expand(env, "Mixed quotes: '$USER' and \"$USER\"", 0);
    test_expand(env, "Nested quotes: \"'$USER'\" and '\"$USER\"'", 0);
    
    printf("\n=== Complex Test Cases ===\n");
    test_expand(env, "echo \"Current user is $USER and home is $HOME\"", 0);
    test_expand(env, "echo 'Single quotes $USER $HOME'", 0);
    test_expand(env, "echo \"Mixed 'quotes' with $USER variable\"", 0);
    test_expand(env, "echo 'Escaped \\$USER in single quotes'", 0);
    test_expand(env, "echo \"Escaped \\$USER in double quotes\"", 0);
    
    printf("\n=== Edge Cases ===\n");
    test_expand(env, "$", 0);
    test_expand(env, "$$", 0);
    test_expand(env, "$?", 127);
    test_expand(env, "$NONEXISTENT", 0);
    test_expand(env, "\\$USER", 0);
    
    env_free(env);
    return 0;
}