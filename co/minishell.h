/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtarza13 <mtarza13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-07-23 10:19:53 by mtarza13          #+#    #+#             */
/*   Updated: 2025-07-23 10:19:53 by mtarza13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <errno.h>

# define SYNTAX_ERROR 2
# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define CTRL_C_EXIT 130
# define CTRL_BACKSLASH_EXIT 131

extern int g_signal_received;

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

// Utils
size_t ft_strlen(const char *s);
char *ft_strdup(const char *s);
char *ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char *ft_strchr(const char *s, int c);
char *ft_strncpy(char *dest, const char *src, size_t n);
void ft_free_array(char **array);
int ft_isalnum(int c);
int ft_isdigit(int c);
int ft_isspace(int c);
int ft_atoi(const char *str);
char *ft_itoa(int n);
int is_valid_identifier(char *str);

// Environment
t_env *init_env(char **envp);
void free_env(t_env *env);
char *get_env_value(t_env *env, char *name);
int set_env_value(t_env *env, char *name, char *value);
int unset_env_value(t_env *env, char *name);
char **env_to_array(t_env *env);

// Expansion
char *expand_variables_advanced(char *str, t_env *env);
char **expand_args_professional(char **args, t_env *env);
char *remove_quotes_advanced(char *str);
char **split_unquoted_words(char *str);

// Lexer
t_token *create_token(t_token_type type, char *value);
void add_token(t_token **tokens, t_token *new_token);
t_token *tokenize(char *input);
void free_tokens(t_token *tokens);

// Parser
t_ast *parse_simple_command(t_token **tokens);
t_ast *parse_pipeline(t_token **tokens);
void free_ast(t_ast *ast);

// Builtins
int builtin_echo(char **args, t_env *env);
int builtin_cd(char **args, t_env *env);
int builtin_pwd(char **args, t_env *env);
int builtin_export(char **args, t_env *env);
int builtin_unset(char **args, t_env *env);
int builtin_env(char **args, t_env *env);
int builtin_exit(char **args, t_env *env);

// Executor
int is_builtin(char *cmd);
int execute_builtin(char **args, t_env *env);
int execute_command(char **args, t_env *env);
int exec_ast(t_ast *ast, t_env *env);

// Signals
void handle_signal(int signo);
void setup_signals(void);

// Main
int minishell_loop(t_shell *shell);

#endif