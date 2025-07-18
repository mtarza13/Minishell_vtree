#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

t_ast_node *ast_command(char **argv) {
    t_ast_node *node;
    node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return 0;
    node->type = AST_COMMAND;
    node->argv = argv;
    node->left = 0;
    node->right = 0;
    node->redir_cmd = 0;
    node->redir_filename = 0;
    return node;
}

t_ast_node *ast_pipe(t_ast_node *left, t_ast_node *right) {
    t_ast_node *node;
    node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return 0;
    node->type = AST_PIPE;
    node->left = left;
    node->right = right;
    node->argv = 0;
    node->redir_cmd = 0;
    node->redir_filename = 0;
    return node;
}

t_ast_node *ast_redir(t_redir_type rtype, char *filename, t_ast_node *cmd) {
    t_ast_node *node;
    node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return 0;
    node->type = AST_REDIR;
    node->redir_type = rtype;
    node->redir_filename = filename;
    node->redir_cmd = cmd;
    node->argv = 0;
    node->left = 0;
    node->right = 0;
    return node;
}

void free_ast(t_ast_node *node) {
    int i;
    if (!node)
        return;
    if (node->type == AST_COMMAND && node->argv) {
        i = 0;
        while (node->argv[i]) {
            free(node->argv[i]);
            i = i + 1;
        }
        free(node->argv);
    }
    if (node->type == AST_PIPE) {
        free_ast(node->left);
        free_ast(node->right);
    }
    if (node->type == AST_REDIR) {
        if (node->redir_filename)
            free(node->redir_filename);
        free_ast(node->redir_cmd);
    }
    free(node);
}

void print_ast(const t_ast_node *node, int level) {
    int i;
    int j;
    const char *rtype[4];
    rtype[0] = "<";
    rtype[1] = ">";
    rtype[2] = ">>";
    rtype[3] = "<<";
    if (!node)
        return;
    i = 0;
    while (i < level) {
        printf("  ");
        i = i + 1;
    }
    if (node->type == AST_COMMAND) {
        printf("COMMAND:");
        j = 0;
        while (node->argv && node->argv[j]) {
            printf(" %s", node->argv[j]);
            j = j + 1;
        }
        printf("\n");
    } else if (node->type == AST_PIPE) {
        printf("PIPE:\n");
        print_ast(node->left, level + 1);
        print_ast(node->right, level + 1);
    } else if (node->type == AST_REDIR) {
        printf("REDIR %s %s\n", rtype[node->redir_type], node->redir_filename);
        print_ast(node->redir_cmd, level + 1);
    }
}
