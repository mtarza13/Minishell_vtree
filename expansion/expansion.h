#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
/**
 * @brief Structure to hold environment variables
 */
typedef struct s_env
{
    char **variables;  /* Array of environment variables (format: KEY=VALUE) */
    int count;         /* Number of environment variables */
} t_env;

/**
 * @brief Initialize environment from envp
 * 
 * @param envp Environment array from main
 * @return t_env* Initialized environment structure
 */
t_env *env_init(char **envp);

/**
 * @brief Get value of environment variable by name
 * 
 * @param env Environment structure
 * @param name Name of the variable to get
 * @return char* Value of the variable (or NULL if not found)
 */
char *env_get(t_env *env, const char *name);

/**
 * @brief Free allocated environment structure
 * 
 * @param env Environment structure to free
 */
void env_free(t_env *env);

/**
 * @brief Detects if position points to a valid variable reference
 * Takes into account quote context
 *
 * @param str Input string to check 
 * @param pos position in the string to check for variable reference
 * @param in_quotes Array tracking quote state
 * @return int 1 if valid variable reference, 0 otherwise
 */
int detect_variable(const char *str, int pos, int *in_quotes);

/**
 * @brief Extracts variable name from string 
 * @param str the input string containing a variable reference
 * @param pos the position of the $ character 
 * @param end_pos pointer to store the position after the variable name
 * @return char* the variable name (without $), must be freed by caller
 */
char *extract_variable(const char *str, int pos, int *end_pos);

/**
 * @brief Look up and return value of variable
 * 
 * @param env Environment structure
 * @param var_name Name of the variable to look up
 * @param exit_status Current exit status (for $?)
 * @return char* Value of the variable (must be freed by caller)
 */
char *lookup_variable(t_env *env, const char *var_name, int exit_status);

/**
 * @brief Expand all variables in a string, respecting quotes
 * 
 * @param env Environment structure
 * @param str String to expand
 * @param exit_status Current exit status
 * @return char* Expanded string (must be freed by caller)
 */
char *expand_variables(t_env *env, const char *str, int exit_status);

#endif
