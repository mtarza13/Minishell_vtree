#include <curses.h>
#include <term.h>
#include <stdio.h>
#include <stdlib.h>
int putchar_tc(int c) {
    return putchar(c);
}

int main() {
    char buffer[2048];
    char *term_type = getenv("TERM");
	printf("%s\n",term_type);
    if (term_type == NULL) {
        printf("TERM not set\n");
        return 1;
    }

    if (tgetent(buffer, term_type) != 1) {
        printf("Could not get terminal entry\n");
        return 1;
    }

    char *cm = tgetstr("cm", NULL); // cursor movement capability
    if (cm == NULL) {
        printf("Terminal has no cursor movement\n");
        return 1;
    }

    // Move cursor to row 5, col 10
    char *move = tgoto(cm, 10, 5);
    tputs(move, 1, putchar_tc);

    printf("Cursor moved!\n");
    return 0;
}

