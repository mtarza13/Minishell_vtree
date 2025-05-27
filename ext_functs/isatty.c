#include <unistd.h>
#include <stdio.h>


int main(void)
{
	if(isatty(STDIN_FILENO))
		printf("Intractive");
	else
		printf("not intractive mode");
}
