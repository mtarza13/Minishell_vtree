#include <unistd.h>
#include <stdio.h>


int main()
{
	char *arg[] = {"/bin/ls","-l",NULL};
	printf("before exec\n");

	execv("/bin/ls",arg);

	printf("after exec \n");
}
