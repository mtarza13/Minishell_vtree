#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
int main() {
    char *line;

    while (1) {
        line = readline("myshell$ ");
        if (!line) break;
        
        if (*line)
            ; // comment out add_history(line);
        
        free(line);
    }

    HIST_ENTRY **entries = history_list();
    if (entries) {
        for (int i = 0; entries[i]; i++)
            printf("History: %s\n", entries[i]->line);
    } else {
        printf("No history saved!\n");
    }

    return 0;
}

