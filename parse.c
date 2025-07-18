#include "parse.h"
#include <stdlib.h>
#include <string.h>

static int count_args(t_token *tok) { 
	int count;
    count = 0;
    while (tok && tok->type == TOKEN_WORD) {
        count = count + 1;
        tok = tok->next;
    }
    return count;
}

static t_ast_node *parse_command(t_token **ptok) {
    int argc;
    char **argv;
    int i;
    t_token *tok;
    argc = count_args(*ptok);
    if (argc == 0)
        return 0;
    argv = (char **)malloc(sizeof(char*) * (argc + 1));
    if (!argv)
        return 0;
    i = 0;
    tok = *ptok;
    while (i < argc) {
        argv[i] = strdup(tok->value);
        tok = tok->next;
        i = i + 1;
    }
    argv[i] = 0;
    *ptok = tok;
    return ast_command(argv);
}

static t_ast_node *parse_redirs(t_token **ptok, t_ast_node *cmd) { 
	t_token *tok;
    t_redir_type rtype;
    char *filename;
    t_ast_node *redir;
    tok = *ptok;
    while (tok && (tok->type == TOKEN_REDIR_OUT || tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_APPEND  || tok->type == TOKEN_REDIR_HEREDOC)) {
        rtype = REDIR_OUT;
        if (tok->type == TOKEN_REDIR_IN)
            rtype = REDIR_IN;
        else if (tok->type == TOKEN_REDIR_APPEND)
           rtype = REDIR_APPEND;
        else if (tok->type == TOKEN_REDIR_HEREDOC) 
			rtype = REDIR_HEREDOC;
        tok = tok->next;
        if (!tok || tok->type != TOKEN_WORD) {
            free_ast(cmd);
            return 0;
        }
        filename = strdup(tok->value);
        tok = tok->next;
        cmd = ast_redir(rtype, filename, cmd);
    }
    *ptok = tok;
    return cmd;
}

t_ast_node *parse_line(t_token *tokens) {
    t_token *tok;
    t_ast_node *cmd;
    t_ast_node *rhs;
    tok = tokens;
    cmd = parse_command(&tok);
    if (!cmd)
        return 0;
    cmd = parse_redirs(&tok, cmd);
    if (tok && tok->type == TOKEN_PIPE) {
        tok = tok->next;
        rhs = parse_line(tok);
        return ast_pipe(cmd, rhs);
    }
    return cmd;
}
