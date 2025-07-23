/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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