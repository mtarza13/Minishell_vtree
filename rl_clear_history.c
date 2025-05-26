
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define HISTORY_FILE ".minishell_history"

int main() {
    char *input;

	while (1) {
        input = readline(">> ");
        if (!input) break; // Ctrl+D

        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        if (strcmp(input, "clear") == 0) {
            rl_clear_history();
            printf("In-memory history cleared.\n");
        } else if (*input) {
            add_history(input);
            printf("You typed: %s\n", input);
        }

        free(input);
    }


    return 0;
}

