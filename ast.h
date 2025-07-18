#ifndef AST_H
#define AST_H

typedef struct s_ast_node t_ast_node;

typedef enum e_ast_type {
    AST_COMMAND,
    AST_PIPE,
    AST_REDIR
} t_ast_type;

typedef enum e_redir_type {
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
	REDIR_HEREDOC
} t_redir_type;

struct s_ast_node {
    t_ast_type type;
    char **argv;
    t_ast_node *left;
    t_ast_node *right;
    t_redir_type redir_type;
    char *redir_filename;
    t_ast_node *redir_cmd;
};

t_ast_node *ast_command(char **argv);
t_ast_node *ast_pipe(t_ast_node *left, t_ast_node *right);
t_ast_node *ast_redir(t_redir_type rtype, char *filename, t_ast_node *cmd);
void free_ast(t_ast_node *node);
void print_ast(const t_ast_node *node, int level);

#endif
