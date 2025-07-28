#include "../include/minishell.h"

// Temporary function to see our tokens
void print_tokens(t_token *tokens)
{
    printf("--- TOKENS ---\n");
    while(tokens)
    {
        printf("Type: %d, Value: [%s]\n", tokens->type, tokens->value);
        tokens = tokens->next;
    }
    printf("--------------\n");
}



// In main.c (or wherever your print function is)

// This is the HELPER function that does the real printing.
// It takes a `prefix` string which holds the indentation.
static void print_tree_recursive(t_ast *node, char *prefix, int is_left)
{
    if (node == NULL)
        return;

    // Print the current node's value
    printf("%s", prefix);
    // This part prints the "├──" or "└──" to show the tree structure
    printf(is_left ? "├──" : "└──" );

    if (node->type == NODE_PIPE)
    {
        printf("PIPE\n");
    }
    else if (node->type == NODE_COMMAND)
    {
        printf("CMD(");
        int i = 0;
        while(node->argv[i])
        {
            printf("\"%s\"", node->argv[i]);
            if (node->argv[i + 1])
                printf(", ");
            i++;
        }
        printf(")\n");
    }

    // --- RECURSIVE CALLS ---
    // Prepare the prefix for the children nodes.
    // The new prefix adds "│   " or "    " depending on whether
    // this node was a left or right child.
    char new_prefix[1024]; // A buffer for the new prefix string
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

    // Recurse for the left and right children
    if (node->left || node->right)
    {
        print_tree_recursive(node->left, new_prefix, 1);
        print_tree_recursive(node->right, new_prefix, 0);
    }
}

// This is the main function you will call from main().
// It starts the process.
void print_ast_as_tree(t_ast *node)
{
    if (node == NULL)
    {
        printf("AST is empty.\n");
        return;
    }
    // Start the recursive printing at the root of the tree.
    // The root has no prefix and we can consider it "not a left child".
    print_tree_recursive(node, "", 0);
}













int main(int argc, char *argv[], char **envp)
{
    char *line;
    t_token *tokens;
	t_ast  *ast_root; // Let's just use one variable for the AST

    (void)argc; (void)argv; (void)envp;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
        {
            add_history(line);
            tokens = tokenaizer(line);
            print_tokens(tokens);

            // --- FIX IS HERE ---
            // 1. Call parse() ONLY ONCE and store the result.
			ast_root = parse(&tokens);

            // 2. Print the result you just got.
            printf("\n\n========= PARSER RESULT =========\n");
            print_ast_as_tree(ast_root); // <-- ADD THIS LINE
            printf("===============================\n\n");
            // --- END FIX ---

	 		// You will need to free the ast_root and tokens later
        }
        free(line);
    }
    return (0);
}

