/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-11 08:03:28 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:14:33 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <errno.h>

#define SYNTAX_ERROR 2
#define COMMAND_NOT_FOUND 127
#define PERMISSION_DENIED 126
#define CTRL_C_EXIT 130
#define CTRL_BACKSLASH_EXIT 131

int g_signal_received = 0;

// ============ STRUCTURES ============
typedef enum e_token_type {
    TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND, TOKEN_HEREDOC, TOKEN_PAREN_OPEN, TOKEN_PAREN_CLOSE,
    TOKEN_EOF, TOKEN_ERROR
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

typedef enum e_node_type { NODE_COMMAND, NODE_PIPE } t_node_type;

typedef struct s_redir {
    t_token_type type;
    char *target;
    struct s_redir *next;
} t_redir;

typedef struct s_ast {
    t_node_type type;
    char **args;
    int arg_count;
    t_redir *redirs;
    struct s_ast *left;
    struct s_ast *right;
} t_ast;

typedef struct s_env {
    char **envp;
    int count;
    int capacity;
    int exit_status;
} t_env;

typedef struct s_shell {
    t_env *env;
    char *pwd;
    char *oldpwd;
} t_shell;

typedef struct s_expand_context {
    int in_single_quote;
    int in_double_quote;
    int preserve_empty;
    int escape_next;
} t_expand_context;

// ============ FUNCTION DECLARATIONS ============
void free_ast(t_ast *ast);
void ft_free_array(char **array);
char *ft_strdup(const char *s);
char *ft_strncpy(char *dest, const char *src, size_t n);
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char *ft_strchr(const char *s, int c);
size_t ft_strlen(const char *s);
int ft_isalnum(int c);
int ft_isdigit(int c);
int ft_isspace(int c);
int ft_atoi(const char *str);
char *ft_itoa(int n);
t_env *init_env(char **envp);
void free_env(t_env *env);
char *get_env_value(t_env *env, char *name);
int set_env_value(t_env *env, char *name, char *value);
int unset_env_value(t_env *env, char *name);
char **env_to_array(t_env *env);
char *expand_variables_advanced(char *str, t_env *env);
char **expand_args_professional(char **args, t_env *env);
int is_valid_identifier(char *str);

// ============ UTILS ============
size_t ft_strlen(const char *s) {
    size_t len = 0;
    if (!s) return 0;
    while (s[len]) len++;
    return len;
}

char *ft_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = ft_strlen(s);
    char *dup = malloc(len + 1);
    if (!dup) return NULL;
    for (size_t i = 0; i <= len; i++) dup[i] = s[i];
    return dup;
}

char *ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2) {
    if (!s1 && !s2) return NULL;
    if (!s1) return free_s2 ? s2 : ft_strdup(s2);
    if (!s2) return free_s1 ? s1 : ft_strdup(s1);
    
    size_t len1 = ft_strlen(s1), len2 = ft_strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len1; i++) result[i] = s1[i];
    for (size_t i = 0; i < len2; i++) result[len1 + i] = s2[i];
    result[len1 + len2] = '\0';
    
    if (free_s1) free(s1);
    if (free_s2) free(s2);
    return result;
}

int ft_strcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return -1;
    while (*s1 && *s2 && *s1 == *s2) { s1++; s2++; }
    return *s1 - *s2;
}

int ft_strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n && s1[i] && s2[i]; i++)
        if (s1[i] != s2[i]) return s1[i] - s2[i];
    return 0;
}

char *ft_strchr(const char *s, int c) {
    if (!s) return NULL;
    while (*s) { if (*s == c) return (char *)s; s++; }
    return c == '\0' ? (char *)s : NULL;
}

char *ft_strncpy(char *dest, const char *src, size_t n) {
    for (size_t i = 0; i < n; i++)
        dest[i] = (i < ft_strlen(src)) ? src[i] : '\0';
    return dest;
}

void ft_free_array(char **array) {
    if (!array) return;
    for (int i = 0; array[i]; i++) free(array[i]);
    free(array);
}

int ft_isalnum(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int ft_isdigit(int c) { return c >= '0' && c <= '9'; }
int ft_isspace(int c) { return c == ' ' || c == '\t' || c == '\n'; }

int ft_atoi(const char *str) {
    int result = 0, sign = 1, i = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)) i++;
    if (str[i] == '-' || str[i] == '+') { if (str[i] == '-') sign = -1; i++; }
    while (str[i] >= '0' && str[i] <= '9') { result = result * 10 + (str[i] - '0'); i++; }
    return result * sign;
}

char *ft_itoa(int n) {
    char *str; int len = 0; long num = n, temp = num;
    if (num <= 0) len = 1;
    while (temp) { temp /= 10; len++; }
    str = malloc(len + 1);
    if (!str) return NULL;
    str[len] = '\0';
    if (num == 0) str[0] = '0';
    if (num < 0) { str[0] = '-'; num = -num; }
    while (num > 0) { str[--len] = (num % 10) + '0'; num /= 10; }
    return str;
}

// ============ ERROR HANDLING ============
int is_valid_identifier(char *str) {
    if (!str || !*str) return 0;
    if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_'))
        return 0;
    for (int i = 1; str[i]; i++)
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || 
              (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
            return 0;
    return 1;
}

// ============ ENVIRONMENT ============
static int count_env(char **envp) {
    int count = 0;
    if (!envp) return 0;
    while (envp[count]) count++;
    return count;
}

t_env *init_env(char **envp) {
    t_env *env = malloc(sizeof(t_env));
    if (!env) return NULL;
    int count = count_env(envp);
    env->capacity = count + 50;
    env->envp = malloc(sizeof(char *) * env->capacity);
    if (!env->envp) { free(env); return NULL; }
    for (int i = 0; i < count; i++) env->envp[i] = ft_strdup(envp[i]);
    for (int i = count; i < env->capacity; i++) env->envp[i] = NULL;
    env->count = count; env->exit_status = 0;
    return env;
}

void free_env(t_env *env) {
    if (!env) return;
    ft_free_array(env->envp);
    free(env);
}

char *get_env_value(t_env *env, char *name) {
    if (!env || !name) return NULL;
    int len = ft_strlen(name);
    for (int i = 0; i < env->count; i++)
        if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0 && env->envp[i][len] == '=')
            return env->envp[i] + len + 1;
    return NULL;
}

int set_env_value(t_env *env, char *name, char *value) {
    if (!env || !name) return 0;
    int len = ft_strlen(name);
    
    // Find existing variable
    for (int i = 0; i < env->count; i++) {
        if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0 && 
            (env->envp[i][len] == '=' || env->envp[i][len] == '\0')) {
            free(env->envp[i]);
            if (value) {
                char *new_var = ft_strjoin_free(ft_strdup(name), ft_strdup("="), 0, 0);
                env->envp[i] = ft_strjoin_free(new_var, ft_strdup(value), 1, 0);
            } else {
                env->envp[i] = ft_strdup(name);
            }
            return 1;
        }
    }
    
    // Add new variable
    if (env->count >= env->capacity - 1) {
        env->capacity *= 2;
        env->envp = realloc(env->envp, sizeof(char*) * env->capacity);
        if (!env->envp) return 0;
        for (int i = env->count; i < env->capacity; i++) env->envp[i] = NULL;
    }
    
    if (value) {
        char *new_var = ft_strjoin_free(ft_strdup(name), ft_strdup("="), 0, 0);
        env->envp[env->count] = ft_strjoin_free(new_var, ft_strdup(value), 1, 0);
    } else {
        env->envp[env->count] = ft_strdup(name);
    }
    env->count++;
    return 1;
}

int unset_env_value(t_env *env, char *name) {
    if (!env || !name) return 0;
    int len = ft_strlen(name);
    for (int i = 0; i < env->count; i++) {
        if (env->envp[i] && ft_strncmp(env->envp[i], name, len) == 0 && 
            (env->envp[i][len] == '=' || env->envp[i][len] == '\0')) {
            free(env->envp[i]);
            for (int j = i; j < env->count - 1; j++) env->envp[j] = env->envp[j + 1];
            env->envp[env->count - 1] = NULL;
            env->count--;
            return 1;
        }
    }
    return 0;
}

char **env_to_array(t_env *env) {
    if (!env) return NULL;
    char **array = malloc(sizeof(char*) * (env->count + 1));
    if (!array) return NULL;
    int j = 0;
    for (int i = 0; i < env->count; i++)
        if (env->envp[i] && ft_strchr(env->envp[i], '='))
            array[j++] = ft_strdup(env->envp[i]);
    array[j] = NULL;
    return array;
}

// ============ ADVANCED VARIABLE EXPANSION - ALL EDGE CASES ============

// Helper function to append character to dynamic string
static char *append_char_dynamic(char *str, char c, int *len, int *capacity) {
    if (*len >= *capacity - 1) {
        *capacity *= 2;
        str = realloc(str, *capacity);
        if (!str) return NULL;
    }
    str[(*len)++] = c;
    str[*len] = '\0';
    return str;
}

// Get variable name with advanced parsing (handles ${VAR}, $VAR, $1, $?, etc.)
static char *get_var_name_complete(char *str, int *i) {
    int start = *i;
    
    // Handle special variables: $?, $$, $0, $1-$9
    if (str[*i] == '?') {
        (*i)++;
        return ft_strdup("?");
    }
    if (str[*i] == '$') {
        (*i)++;
        return ft_strdup("$");
    }
    if (str[*i] == '0') {
        (*i)++;
        return ft_strdup("0");
    }
    if (ft_isdigit(str[*i])) {
        (*i)++;
        char *name = malloc(2);
        if (!name) return NULL;
        name[0] = str[start];
        name[1] = '\0';
        return name;
    }
    
    // Handle braced variables: ${VAR}
    if (str[*i] == '{') {
        (*i)++; // Skip '{'
        start = *i;
        while (str[*i] && str[*i] != '}') (*i)++;
        if (str[*i] == '}') {
            int len = *i - start;
            (*i)++; // Skip '}'
            if (len == 0) return ft_strdup("");
            char *name = malloc(len + 1);
            if (!name) return NULL;
            ft_strncpy(name, str + start, len);
            name[len] = '\0';
            return name;
        }
        // Malformed ${VAR without closing brace, treat as literal
        *i = start - 1; // Reset to '{' position
        return NULL;
    }
    
    // Handle regular variables: $VAR, $VAR_NAME
    if (ft_isalnum(str[*i]) || str[*i] == '_') {
        while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_')) (*i)++;
        int len = *i - start;
        char *name = malloc(len + 1);
        if (!name) return NULL;
        ft_strncpy(name, str + start, len);
        name[len] = '\0';
        return name;
    }
    
    // Invalid character after $, return NULL to treat $ as literal
    return NULL;
}

// Expand special and environment variables
static char *expand_variable_value(char *var_name, t_env *env) {
    if (!var_name) return ft_strdup("");
    
    // Special variables
    if (ft_strcmp(var_name, "?") == 0) {
        return ft_itoa(env->exit_status);
    }
    if (ft_strcmp(var_name, "$") == 0) {
        return ft_itoa(getpid());
    }
    if (ft_strcmp(var_name, "0") == 0) {
        return ft_strdup("minishell");
    }
    
    // Positional parameters $1-$9 (always empty in minishell)
    if (ft_isdigit(var_name[0]) && ft_strlen(var_name) == 1) {
        return ft_strdup("");
    }
    
    // Environment variable
    char *value = get_env_value(env, var_name);
    return value ? ft_strdup(value) : ft_strdup("");
}

// Advanced variable expansion that handles ALL edge cases
char *expand_variables_advanced(char *str, t_env *env) {
    if (!str) return NULL;
    
    int capacity = ft_strlen(str) * 2 + 100;
    char *result = malloc(capacity);
    if (!result) return NULL;
    result[0] = '\0';
    
    int result_len = 0;
    int i = 0;
    t_expand_context ctx = {0, 0, 0, 0};
    
    while (str[i]) {
        // Handle escape sequences first
        if (ctx.escape_next) {
            result = append_char_dynamic(result, str[i], &result_len, &capacity);
            ctx.escape_next = 0;
            i++;
            continue;
        }
        
        // Handle backslash escaping
        if (str[i] == '\\' && !ctx.in_single_quote) {
            if (str[i + 1] && (str[i + 1] == '$' || str[i + 1] == '"' || 
                              str[i + 1] == '\\' || str[i + 1] == '\n' ||
                              (ctx.in_double_quote && str[i + 1] == '`'))) {
                ctx.escape_next = 1;
                i++; // Skip backslash
                continue;
            } else if (!ctx.in_double_quote) {
                // In unquoted context, backslash is literal if not escaping special char
                result = append_char_dynamic(result, str[i], &result_len, &capacity);
                i++;
                continue;
            }
        }
        
        // Handle quote state changes
        if (str[i] == '\'' && !ctx.in_double_quote) {
            ctx.in_single_quote = !ctx.in_single_quote;
            result = append_char_dynamic(result, str[i], &result_len, &capacity);
            i++;
            continue;
        }
        
        if (str[i] == '"' && !ctx.in_single_quote) {
            ctx.in_double_quote = !ctx.in_double_quote;
            result = append_char_dynamic(result, str[i], &result_len, &capacity);
            ctx.preserve_empty = 1;
            i++;
            continue;
        }
        
        // Handle variable expansion (not in single quotes)
        if (str[i] == '$' && !ctx.in_single_quote) {
            // Handle escaped $ (already handled above, but double check)
            if (i > 0 && str[i-1] == '\\') {
                result = append_char_dynamic(result, str[i], &result_len, &capacity);
                i++;
                continue;
            }
            
            // Look ahead for variable pattern
            if (!str[i + 1]) {
                // Lone $ at end of string
                result = append_char_dynamic(result, str[i], &result_len, &capacity);
                i++;
                continue;
            }
            
            i++; // Skip '$'
            
            // Get variable name
            char *var_name = get_var_name_complete(str, &i);
            if (var_name) {
                // Valid variable found, expand it
                char *var_value = expand_variable_value(var_name, env);
                if (var_value) {
                    // Append expanded value to result
                    for (int j = 0; var_value[j]; j++) {
                        result = append_char_dynamic(result, var_value[j], &result_len, &capacity);
                    }
                    free(var_value);
                }
                free(var_name);
            } else {
                // Invalid variable name or lone $, treat $ as literal
                result = append_char_dynamic(result, '$', &result_len, &capacity);
                // Don't increment i, process current character normally
            }
            continue;
        }
        
        // Regular character
        result = append_char_dynamic(result, str[i], &result_len, &capacity);
        i++;
    }
    
    return result;
}

// Quote removal with proper handling of escape sequences
char *remove_quotes_advanced(char *str) {
    if (!str) return NULL;
    
    int capacity = ft_strlen(str) + 1;
    char *result = malloc(capacity);
    if (!result) return NULL;
    
    int i = 0, j = 0;
    int in_single_quote = 0, in_double_quote = 0;
    
    while (str[i]) {
        if (str[i] == '\'' && !in_double_quote) {
            in_single_quote = !in_single_quote;
            // Don't add quote to result
        } else if (str[i] == '"' && !in_single_quote) {
            in_double_quote = !in_double_quote;
            // Don't add quote to result
        } else if (str[i] == '\\' && !in_single_quote) {
            // Handle escape sequences
            if (str[i + 1] && (str[i + 1] == '$' || str[i + 1] == '"' || 
                              str[i + 1] == '\\' || str[i + 1] == '\n' ||
                              (in_double_quote && str[i + 1] == '`'))) {
                i++; // Skip backslash
                result[j++] = str[i]; // Add escaped character
            } else {
                result[j++] = str[i]; // Keep backslash if not escaping
            }
        } else {
            result[j++] = str[i];
        }
        i++;
    }
    
    result[j] = '\0';
    return result;
}

// Word splitting with proper whitespace handling
char **split_unquoted_words(char *str) {
    if (!str || !*str) return NULL;
    
    // Count words
    int word_count = 0;
    int in_word = 0;
    
    for (int i = 0; str[i]; i++) {
        if (ft_isspace(str[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }
    
    if (word_count == 0) return NULL;
    
    char **words = malloc(sizeof(char*) * (word_count + 1));
    if (!words) return NULL;
    
    int word_idx = 0;
    int i = 0;
    
    while (word_idx < word_count && str[i]) {
        // Skip whitespace
        while (str[i] && ft_isspace(str[i])) i++;
        if (!str[i]) break;
        
        int start = i;
        // Find end of word
        while (str[i] && !ft_isspace(str[i])) i++;
        
        int len = i - start;
        words[word_idx] = malloc(len + 1);
        if (!words[word_idx]) {
            ft_free_array(words);
            return NULL;
        }
        
        ft_strncpy(words[word_idx], str + start, len);
        words[word_idx][len] = '\0';
        word_idx++;
    }
    
    words[word_idx] = NULL;
    return words;
}

// Professional argument expansion with all edge cases
char **expand_args_professional(char **args, t_env *env) {
    if (!args) return NULL;
    
    int original_count = 0;
    while (args[original_count]) original_count++;
    
    // Pre-allocate generous space for result
    int result_capacity = original_count * 4 + 10;
    char **result = malloc(sizeof(char*) * result_capacity);
    if (!result) return NULL;
    
    int result_count = 0;
    
    for (int i = 0; i < original_count; i++) {
        // Check if argument contains quotes (to determine splitting behavior)
        int has_quotes = (ft_strchr(args[i], '"') != NULL || 
                         ft_strchr(args[i], '\'') != NULL);
        
        // Step 1: Expand variables
        char *expanded = expand_variables_advanced(args[i], env);
        if (!expanded) expanded = ft_strdup("");
        
        // Step 2: Remove quotes
        char *unquoted = remove_quotes_advanced(expanded);
        free(expanded);
        if (!unquoted) unquoted = ft_strdup("");
        
        // Step 3: Word splitting (only for unquoted arguments)
        if (has_quotes || ft_strchr(args[i], '"') || ft_strchr(args[i], '\'')) {
            // Quoted argument - don't split, preserve as single word
            if (result_count >= result_capacity - 1) {
                result_capacity *= 2;
                result = realloc(result, sizeof(char*) * result_capacity);
                if (!result) return NULL;
            }
            result[result_count++] = unquoted;
        } else {
            // Unquoted argument - may need word splitting
            char **split_words = split_unquoted_words(unquoted);
            if (split_words) {
                int split_count = 0;
                while (split_words[split_count]) split_count++;
                
                // Ensure enough space in result array
                while (result_count + split_count >= result_capacity) {
                    result_capacity *= 2;
                    result = realloc(result, sizeof(char*) * result_capacity);
                    if (!result) return NULL;
                }
                
                // Add all split words
                for (int j = 0; j < split_count; j++) {
                    result[result_count++] = split_words[j];
                }
                free(split_words); // Free array but not strings
                free(unquoted);
            } else {
                // No words after splitting (empty or whitespace only)
                if (*unquoted) {
                    // Non-empty result, keep it
                    if (result_count >= result_capacity - 1) {
                        result_capacity *= 2;
                        result = realloc(result, sizeof(char*) * result_capacity);
                        if (!result) return NULL;
                    }
                    result[result_count++] = unquoted;
                } else {
                    // Empty result, discard
                    free(unquoted);
                }
            }
        }
    }
    
    result[result_count] = NULL;
    return result;
}

// ============ LEXER ============
t_token *create_token(t_token_type type, char *value) {
    t_token *token = malloc(sizeof(t_token));
    if (!token) return NULL;
    token->type = type;
    token->value = value ? ft_strdup(value) : NULL;
    token->next = NULL;
    return token;
}

void add_token(t_token **tokens, t_token *new_token) {
    if (!*tokens) { *tokens = new_token; return; }
    t_token *current = *tokens;
    while (current->next) current = current->next;
    current->next = new_token;
}

t_token *tokenize(char *input) {
    if (!input) return NULL;
    t_token *tokens = NULL;
    int i = 0;
    
    while (input[i]) {
        while (input[i] == ' ' || input[i] == '\t') i++;
        if (!input[i]) break;
        
        if (input[i] == '|') {
            add_token(&tokens, create_token(TOKEN_PIPE, "|")); 
            i++;
        } else if (input[i] == '<') {
            if (input[i+1] == '<') {
                add_token(&tokens, create_token(TOKEN_HEREDOC, "<<")); 
                i += 2;
            } else {
                add_token(&tokens, create_token(TOKEN_REDIR_IN, "<")); 
                i++;
            }
        } else if (input[i] == '>') {
            if (input[i+1] == '>') {
                add_token(&tokens, create_token(TOKEN_REDIR_APPEND, ">>")); 
                i += 2;
            } else {
                add_token(&tokens, create_token(TOKEN_REDIR_OUT, ">")); 
                i++;
            }
        } else if (input[i] == '(') {
            add_token(&tokens, create_token(TOKEN_PAREN_OPEN, "(")); 
            i++;
        } else if (input[i] == ')') {
            add_token(&tokens, create_token(TOKEN_PAREN_CLOSE, ")")); 
            i++;
        } else {
            int start = i;
            int in_single_quote = 0, in_double_quote = 0;
            
            while (input[i]) {
                if (!in_single_quote && !in_double_quote &&
                    (input[i] == ' ' || input[i] == '\t' ||
                     input[i] == '|' || input[i] == '<' || input[i] == '>' ||
                     input[i] == '(' || input[i] == ')'))
                    break;
                    
                if (input[i] == '\'' && !in_double_quote)
                    in_single_quote = !in_single_quote;
                else if (input[i] == '"' && !in_single_quote)
                    in_double_quote = !in_double_quote;
                else if (input[i] == '\\' && input[i + 1])
                    i++; // Skip escaped character
                i++;
            }
            
            int len = i - start;
            if (len > 0) {
                char *word = malloc(len + 1);
                if (!word) break;
                ft_strncpy(word, input + start, len);
                word[len] = '\0';
                add_token(&tokens, create_token(TOKEN_WORD, word));
                free(word);
            }
        }
    }
    return tokens;
}

void free_tokens(t_token *tokens) {
    while (tokens) {
        t_token *tmp = tokens;
        tokens = tokens->next;
        if (tmp->value) free(tmp->value);
        free(tmp);
    }
}

// ============ AST FUNCTIONS ============
void free_ast(t_ast *ast) {
    if (!ast) return;
    if (ast->args) ft_free_array(ast->args);
    t_redir *redir = ast->redirs, *tmp;
    while (redir) {
        tmp = redir;
        redir = redir->next;
        if (tmp->target) free(tmp->target);
        free(tmp);
    }
    free_ast(ast->left);
    free_ast(ast->right);
    free(ast);
}

// ============ PARSER ============
int count_args(t_token *tokens) {
    int count = 0;
    t_token *cur = tokens;
    while (cur && cur->type == TOKEN_WORD) {
        count++;
        cur = cur->next;
    }
    return count;
}

char **copy_args(t_token *tokens, int argc) {
    char **args = malloc(sizeof(char*) * (argc + 1));
    for (int i = 0; i < argc; i++) {
        args[i] = ft_strdup(tokens->value);
        tokens = tokens->next;
    }
    args[argc] = NULL;
    return args;
}

t_ast *parse_simple_command(t_token **tokens) {
    int argc = count_args(*tokens);
    if (argc == 0) return NULL;
    
    t_ast *cmd = malloc(sizeof(t_ast));
    if (!cmd) return NULL;
    
    cmd->type = NODE_COMMAND;
    cmd->args = copy_args(*tokens, argc);
    cmd->arg_count = argc;
    cmd->redirs = NULL;
    cmd->left = cmd->right = NULL;
    
    for (int i = 0; i < argc; i++) 
        (*tokens) = (*tokens)->next;
    
    return cmd;
}

t_ast *parse_pipeline(t_token **tokens) {
    t_ast *left = parse_simple_command(tokens);
    if (!left) return NULL;
    
    while (*tokens && (*tokens)->type == TOKEN_PIPE) {
        *tokens = (*tokens)->next; // Skip pipe
        t_ast *right = parse_simple_command(tokens);
        if (!right) {
            free_ast(left);
            return NULL;
        }
        
        t_ast *pipe = malloc(sizeof(t_ast));
        if (!pipe) {
            free_ast(left);
            free_ast(right);
            return NULL;
        }
        
        pipe->type = NODE_PIPE;
        pipe->args = NULL;
        pipe->arg_count = 0;
        pipe->redirs = NULL;
        pipe->left = left;
        pipe->right = right;
        left = pipe;
    }
    return left;
}

// ============ BUILTINS ============

// Helper function to sort environment variables for export display
void sort_env_for_export(char **env_array, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (env_array[j] && env_array[j + 1] && 
                ft_strcmp(env_array[j], env_array[j + 1]) > 0) {
                char *temp = env_array[j];
                env_array[j] = env_array[j + 1];
                env_array[j + 1] = temp;
            }
        }
    }
}

// Print environment variable in export format
void print_export_format(char *env_var) {
    char *equals = ft_strchr(env_var, '=');
    if (equals) {
        *equals = '\0';
        printf("declare -x %s=\"%s\"\n", env_var, equals + 1);
        *equals = '=';
    } else {
        printf("declare -x %s\n", env_var);
    }
}

// Complete export builtin implementation
int builtin_export(char **args, t_env *env) {
    // Case 1: export without arguments - display all exported variables
    if (!args[1]) {
        char **sorted_env = malloc(sizeof(char*) * (env->count + 1));
        if (!sorted_env) return 1;
        
        int j = 0;
        for (int i = 0; i < env->count; i++) {
            if (env->envp[i]) {
                sorted_env[j++] = ft_strdup(env->envp[i]);
            }
        }
        sorted_env[j] = NULL;
        
        sort_env_for_export(sorted_env, j);
        
        for (int i = 0; i < j; i++) {
            if (sorted_env[i]) {
                print_export_format(sorted_env[i]);
            }
        }
        
        ft_free_array(sorted_env);
        return 0;
    }
    
    // Case 2: export with arguments
    int exit_status = 0;
    for (int i = 1; args[i]; i++) {
        char *arg = args[i];
        char *equals = ft_strchr(arg, '=');
        
        if (equals) {
            // Case: export VAR=value
            *equals = '\0';
            char *name = arg;
            char *value = equals + 1;
            
            if (!is_valid_identifier(name)) {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
                exit_status = 1;
                *equals = '='; // Restore original string
                continue;
            }
            
            if (!set_env_value(env, name, value)) {
                fprintf(stderr, "minishell: export: failed to set variable\n");
                exit_status = 1;
            }
            
            *equals = '='; // Restore original string
        } else {
            // Case: export VAR (without value)
            if (!is_valid_identifier(arg)) {
                fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
                exit_status = 1;
                continue;
            }
            
            // Check if variable already exists
            char *existing_value = get_env_value(env, arg);
            if (existing_value) {
                // Variable exists, keep its value but mark as exported
                if (!set_env_value(env, arg, existing_value)) {
                    fprintf(stderr, "minishell: export: failed to export variable\n");
                    exit_status = 1;
                }
            } else {
                // Variable doesn't exist, create it without value
                if (!set_env_value(env, arg, NULL)) {
                    fprintf(stderr, "minishell: export: failed to export variable\n");
                    exit_status = 1;
                }
            }
        }
    }
    
    return exit_status;
}

int builtin_unset(char **args, t_env *env) {
    if (!args[1]) {
        fprintf(stderr, "minishell: unset: not enough arguments\n");
        return 1;
    }
    
    int exit_status = 0;
    for (int i = 1; args[i]; i++) {
        if (!is_valid_identifier(args[i])) {
            fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n", args[i]);
            exit_status = 1;
            continue;
        }
        
        unset_env_value(env, args[i]);
    }
    
    return exit_status;
}

int builtin_echo(char **args, t_env *env) {
    (void)env;
    int i = 1;
    int newline = 1;
    
    if (args[1] && ft_strcmp(args[1], "-n") == 0) {
        newline = 0;
        i = 2;
    }
    
    while (args[i]) {
        printf("%s", args[i]);
        if (args[i + 1]) printf(" ");
        i++;
    }
    if (newline) printf("\n");
    return 0;
}

int builtin_pwd(char **args, t_env *env) {
    (void)args;
    (void)env;
    char *pwd = getcwd(NULL, 0);
    if (pwd) {
        printf("%s\n", pwd);
        free(pwd);
        return 0;
    }
    return 1;
}

int builtin_env(char **args, t_env *env) {
    (void)args;
    for (int i = 0; i < env->count; i++) {
        if (env->envp[i] && ft_strchr(env->envp[i], '='))
            printf("%s\n", env->envp[i]);
    }
    return 0;
}

int builtin_cd(char **args, t_env *env) {
    char *path;
    char *old_pwd = getcwd(NULL, 0);
    
    if (!args[1] || ft_strcmp(args[1], "~") == 0) {
        // cd or cd ~ - go to HOME
        path = get_env_value(env, "HOME");
        if (!path) {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            if (old_pwd) free(old_pwd);
            return 1;
        }
    } else if (ft_strcmp(args[1], "-") == 0) {
        // cd - - go to OLDPWD
        path = get_env_value(env, "OLDPWD");
        if (!path) {
            fprintf(stderr, "minishell: cd: OLDPWD not set\n");
            if (old_pwd) free(old_pwd);
            return 1;
        }
        printf("%s\n", path);
    } else {
        path = args[1];
    }
    
    if (chdir(path) == -1) {
        fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
        if (old_pwd) free(old_pwd);
        return 1;
    }
    
    // Update OLDPWD and PWD
    if (old_pwd) {
        set_env_value(env, "OLDPWD", old_pwd);
        free(old_pwd);
    }
    
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd) {
        set_env_value(env, "PWD", new_pwd);
        free(new_pwd);
    }
    
    return 0;
}

int builtin_exit(char **args, t_env *env) {
    (void)env;
    printf("exit\n");
    if (args[1]) {
        int code = ft_atoi(args[1]);
        exit(code);
    }
    exit(0);
}

// ============ EXECUTOR ============
int is_builtin(char *cmd) {
    return (ft_strcmp(cmd, "echo") == 0 || 
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

int execute_builtin(char **args, t_env *env) {
    if (ft_strcmp(args[0], "echo") == 0)
        return builtin_echo(args, env);
    if (ft_strcmp(args[0], "cd") == 0)
        return builtin_cd(args, env);
    if (ft_strcmp(args[0], "pwd") == 0)
        return builtin_pwd(args, env);
    if (ft_strcmp(args[0], "export") == 0)
        return builtin_export(args, env);
    if (ft_strcmp(args[0], "unset") == 0)
        return builtin_unset(args, env);
    if (ft_strcmp(args[0], "env") == 0)
        return builtin_env(args, env);
    if (ft_strcmp(args[0], "exit") == 0)
        return builtin_exit(args, env);
    return 1;
}

int execute_command(char **args, t_env *env) {
    if (!args || !args[0]) return 0;
    
    if (is_builtin(args[0])) {
        return execute_builtin(args, env);
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        char **envp = env_to_array(env);
        execvp(args[0], args);
        fprintf(stderr, "minishell: %s: %s\n", args[0], strerror(errno));
        ft_free_array(envp);
        exit(errno == EACCES ? PERMISSION_DENIED : COMMAND_NOT_FOUND);
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        env->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
        return env->exit_status;
    }
    return 1;
}

int exec_ast(t_ast *ast, t_env *env) {
    if (!ast) return 1;
    
    if (ast->type == NODE_COMMAND) {
        char **expanded = expand_args_professional(ast->args, env);
        int status = execute_command(expanded, env);
        ft_free_array(expanded);
        return status;
    } else if (ast->type == NODE_PIPE) {
        int fd[2], status = 0;
        if (pipe(fd) == -1) return 1;
        
        pid_t pid = fork();
        if (pid == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            status = exec_ast(ast->left, env);
            exit(status);
        } else if (pid > 0) {
            close(fd[1]);
            int saved_stdin = dup(STDIN_FILENO);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            status = exec_ast(ast->right, env);
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
            int wstatus;
            waitpid(pid, &wstatus, 0);
            env->exit_status = WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : 1;
            return status;
        }
    }
    return 1;
}

// ============ SIGNAL HANDLING ============
void handle_signal(int signo) {
    if (signo == SIGINT) {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal_received = CTRL_C_EXIT;
    }
}

void setup_signals(void) {
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, SIG_IGN);
}

// ============ MAIN LOOP ============
int minishell_loop(t_shell *shell) {
    char *line;
    t_token *tokens = NULL;
    t_ast *ast = NULL;
    
    while (1) {
        setup_signals();
        line = readline("minishell$ ");
        
        if (!line) {
            printf("exit\n");
            break;
        }
        
        if (*line)
            add_history(line);
            
        tokens = tokenize(line);
        t_token *token_cursor = tokens;
        ast = parse_pipeline(&token_cursor);
        
        if (ast) {
            exec_ast(ast, shell->env);
        }
        
        free_tokens(tokens);
        free_ast(ast);
        free(line);
    }
    return 0;
}

// ============ MAIN ============
int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;
    
    t_shell shell;
    shell.env = init_env(envp);
    shell.pwd = getcwd(NULL, 0);
    shell.oldpwd = NULL;
    
    if (!shell.env) {
        fprintf(stderr, "Error: Failed to initialize environment\n");
        return 1;
    }
    
    minishell_loop(&shell);
    
    free_env(shell.env);
    if (shell.pwd) free(shell.pwd);
    if (shell.oldpwd) free(shell.oldpwd);
    
    return 0;
}
