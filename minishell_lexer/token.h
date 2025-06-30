#ifndef TOKEN_H
#define TOKEN_H

typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    APPEND,
    HEREDOC
}   t_token_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
    struct s_token *next;
}   t_token;

t_token *tokenize_input(char *input);

#endif
