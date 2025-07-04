#include "expansion.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initialize environment from envp
 */
t_env *env_init(char **envp)
{
    t_env *env;
    int i;
    
    if (!envp)
        return NULL;
    
    env = (t_env *)malloc(sizeof(t_env));
    if (!env)
        return NULL;
    
    for (i = 0; envp[i]; i++);
    
    env->count = i;
    env->variables = (char **)malloc(sizeof(char *) * (i + 1));
    if (!env->variables)
    {
        free(env);
        return NULL;
    }
    
    for (i = 0; envp[i]; i++)
    {
        env->variables[i] = strdup(envp[i]);
        if (!env->variables[i])
        {
            while (--i >= 0)
                free(env->variables[i]);
            free(env->variables);
            free(env);
            return NULL;
        }
    }
    env->variables[i] = NULL;
    
    return env;
}

/**
 * @brief Get value of environment variable by name
 */
char *env_get(t_env *env, const char *name)
{
    int i;
    size_t len;
    
    if (!env || !name)
        return NULL;
    
    len = strlen(name);
    for (i = 0; i < env->count; i++)
    {
        if (strncmp(env->variables[i], name, len) == 0 && 
            env->variables[i][len] == '=')
        {
            return &env->variables[i][len + 1];
        }
    }
    
    return NULL;
}

/**
 * @brief Free allocated environment structure
 */
void env_free(t_env *env)
{
    int i;
    
    if (!env)
        return;
    
    if (env->variables)
    {
        for (i = 0; i < env->count; i++)
        {
            if (env->variables[i])
                free(env->variables[i]);
        }
        free(env->variables);
    }
    
    free(env);
}