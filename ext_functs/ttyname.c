#include <stdio.h>
#include <unistd.h>


int main()
{
	char *tty = ttyname(1);
	printf("%shh",tty);
}
