#include "lexer.h"

int check_syntax_error(t_token *token)
{
    if (!token) return 0;

    t_token *current = token;
    
    // Check if starts with pipe
    if (current->type == TOKEN_PIPE)
    {
        printf("bash: syntax error near unexpected token '|'\n");
        return 0;
    }
    
    while (current)
    {
        // Check redirections have file after them
        if (current->type >= TOKEN_REDIR_IN && current->type <= TOKEN_REDIR_HEREDOC)
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                if (current->next)
                    printf("bash: syntax error near unexpected token '%s'\n", current->next->value);
                else
                    printf("bash: syntax error near unexpected token 'newline'\n");
                return 0;
            }
        }
        
        // Check for pipe at end
        if (current->type == TOKEN_PIPE && !current->next)
        {
            printf("bash: syntax error near unexpected token 'newline'\n");
            return 0;
        }
        
        current = current->next;
    }
    
    return 1;
}

static t_token *new_token(char *tok, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->type = type;
    token->value = tok;
    token->next = NULL;
    return token;
}

t_token_type get_type(char *tok)
{
    if (strcmp(tok, ">>") == 0)
        return TOKEN_REDIR_APPEND;
    if (strcmp(tok, "<<") == 0)
        return TOKEN_REDIR_HEREDOC;
    if (strcmp(tok, "&&") == 0)
        return TOKEN_AND;
    if (strcmp(tok, "||") == 0)
        return TOKEN_OR;
    if (strcmp(tok, "|") == 0)
        return TOKEN_PIPE;
    if (strcmp(tok, ">") == 0)
        return TOKEN_REDIR_OUT;
    if (strcmp(tok, "<") == 0)
        return TOKEN_REDIR_IN;
    if (strcmp(tok, ";") == 0)
        return TOKEN_SEMICOLON;
    return TOKEN_WORD;
}

void add_token(t_token **head, t_token *node)
{
    if (!*head)
    {
        *head = node;
    }
    else
    {
        t_token *tmp = *head;
        while (tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = node;
    }
}

void print_tokens(t_token *token)
{
    while (token)
    {
        printf("token [%-15s] | type %d | len = %zu\n",
            token->value, token->type, strlen(token->value));
        token = token->next;
    }
}

char *extract_quoted(const char **ptr)
{
    char quote = **ptr;
    (*ptr)++;
    const char *start = *ptr;
    
    while (**ptr && **ptr != quote)
        (*ptr)++;

    if (**ptr != quote)
    {
        printf("Error: Unclosed quote\n");
        return NULL;
    }

    char *tok = strndup(start, *ptr - start);
    (*ptr)++;
    return tok;
}

static int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == ';' || c == '&');
}

int check_quote_balance(const char *input)
{
    int single_quote = 0, double_quote = 0;

    while (*input)
    {
        if (*input == '\'' && !double_quote)
            single_quote = !single_quote;
        else if (*input == '"' && !single_quote)
            double_quote = !double_quote;
        
        input++;
    }
    
    if (single_quote || double_quote)
    {
        printf("Error: unclosed quote\n");
        return 0;
    }
    return 1;
}

char *remove_line_continuations(const char *input)
{
    int len = strlen(input);
    char *result = malloc(len + 1);
    int i = 0, j = 0;
    
    while (i < len)
    {
        if (input[i] == '\\' && input[i + 1] == '\n')
        {
            // Skip backslash + newline
            i += 2;
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return result;
}

char *handle_escape_outside_quotes(const char **ptr)
{
    char *result = malloc(1024);
    int i = 0;
    
    while (**ptr && **ptr != ' ' && **ptr != '\t' && **ptr != '\n' && 
           **ptr != '\'' && **ptr != '"' && !is_operator_char(**ptr))
    {
        if (**ptr == '\\')
        {
            (*ptr)++;
            if (**ptr)
            {
                result[i++] = **ptr;
                (*ptr)++;
            }
        }
        else
        {
            result[i++] = **ptr;
            (*ptr)++;
        }
    }
    result[i] = '\0';
    return result;
}

char *expand_variables(const char *word)
{
    // Simple variable expansion - you can enhance this
    if (strcmp(word, "$USER") == 0)
        return strdup("mtarza13");
    if (strcmp(word, "$HOME") == 0)
        return strdup("/home/mtarza13");
    if (strcmp(word, "$?") == 0)
        return strdup("0");
    
    return strdup(word);
}

t_token *lexer(const char *input)
{
    if (!input)
        return NULL;
        
    char *cleaned_input = remove_line_continuations(input);
    t_token *token = NULL;
    const char *ptr = cleaned_input;
    
    if (!check_quote_balance(ptr))
    {
        free(cleaned_input);
        return NULL;
    }

    while (*ptr)
    {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
            ptr++;
        if (!*ptr) break;

        char *word = NULL;
        
        // Handle quotes
        if (*ptr == '\'' || *ptr == '"')
        {
            word = extract_quoted(&ptr);
            if (!word)
            {
                free_tokens(token);
                free(cleaned_input);
                return NULL;
            }
        }
        // Handle operators
        else if (is_operator_char(*ptr))
        {
            const char *start = ptr;
            ptr++;
            // Handle double operators: >>, <<, &&, ||
            if ((*start == '>' && *ptr == '>') || 
                (*start == '<' && *ptr == '<') ||
                (*start == '&' && *ptr == '&') ||
                (*start == '|' && *ptr == '|'))
                ptr++;
            word = strndup(start, ptr - start);
        }
        // Handle regular words with escape
        else
        {
            const char *start = ptr;
            if (strchr(start, '\\'))
            {
                word = handle_escape_outside_quotes(&ptr);
            }
            else
            {
                while (*ptr && *ptr != '\'' && *ptr != '\n' && *ptr != '"' && 
                       *ptr != '\t' && *ptr != ' ' && !is_operator_char(*ptr))
                    ptr++;
                word = strndup(start, ptr - start);
            }
        }
        
        if (word && strlen(word) > 0)
        {
            // Variable expansion for words starting with $
            if (word[0] == '$')
            {
                char *expanded = expand_variables(word);
                free(word);
                word = expanded;
            }
            
            t_token_type type = get_type(word);
            add_token(&token, new_token(word, type));
        }
        else
        {
            free_tokens(token);
            free(word);
            free(cleaned_input);
            return NULL;
        }
    }
    
    free(cleaned_input);
    
    if (!check_syntax_error(token))
    {
        free_tokens(token);
        return NULL;
    }
    
    return token;
}

void free_tokens(t_token *token)
{
    while (token)
    {
        t_token *next = token->next;
        free(token->value);
        free(token);
        token = next;
    }
}
