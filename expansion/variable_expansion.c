#include "expansion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Look up and return value of variable
 */
char *lookup_variable(t_env *env, const char *var_name, int exit_status)
{
    char exit_status_str[12];
    
    if (!var_name)
        return NULL;
    
    if (strcmp(var_name, "?") == 0)
    {
        sprintf(exit_status_str, "%d", exit_status);
        return strdup(exit_status_str);
    }
    
    if (env)
    {
        char *value = env_get(env, var_name);
        if (value)
            return strdup(value);
    }
    
    return strdup("");
}

/**
 * @brief Helper function to update quote state
 */
static void update_quote_state(char c, int *in_quotes)
{
    if (c == '\'')
    {
        if (!in_quotes[1])
            in_quotes[0] = !in_quotes[0];
    }
    else if (c == '"')
    {
        if (!in_quotes[0])
            in_quotes[1] = !in_quotes[1];
    }
}

/**
 * @brief Helper function to calculate length of expanded string
 */
static size_t calculate_expanded_length(t_env *env, const char *str, int exit_status)
{
    size_t len = 0;
    int i = 0;
    int in_quotes[2] = {0, 0};
    
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            update_quote_state(str[i], in_quotes);
            len++;
            i++;
        }
        else if (str[i] == '$' && detect_variable(str, i, in_quotes))
        {
            int end_pos;
            char *var_name = extract_variable(str, i, &end_pos);
            if (var_name)
            {
                char *value = lookup_variable(env, var_name, exit_status);
                if (value)
                {
                    len += strlen(value);
                    free(value);
                }
                free(var_name);
                i = end_pos;
            }
            else
                len++, i++;
        }
        else
            len++, i++;
    }
    
    return len;
}

/**
 * @brief Expand all variables in a string, respecting quotes
 */
char *expand_variables(t_env *env, const char *str, int exit_status)
{
    char *result;
    size_t result_len;
    int i = 0, j = 0;
    int in_quotes[2] = {0, 0};
    
    if (!str)
        return NULL;
    
    result_len = calculate_expanded_length(env, str, exit_status);
    
    result = (char *)malloc(result_len + 1);
    if (!result)
        return NULL;
    
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            update_quote_state(str[i], in_quotes);
            result[j++] = str[i++];
        }
        else if (str[i] == '$' && detect_variable(str, i, in_quotes))
        {
            int end_pos;
            char *var_name = extract_variable(str, i, &end_pos);
            if (var_name)
            {
                char *value = lookup_variable(env, var_name, exit_status);
                if (value)
                {
                    strcpy(&result[j], value);
                    j += strlen(value);
                    free(value);
                }
                free(var_name);
                i = end_pos;
            }
            else
                result[j++] = str[i++];
        }
        else
            result[j++] = str[i++];
    }
    
    result[j] = '\0';
    return result;
}