#include <stdio.h>
#include <unistd.h>


int main()
{
	char *tty = ttyname(0);
	printf("%shh",tty);
}
