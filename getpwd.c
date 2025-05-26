#include <stdio.h>
#include <unistd.h>

int main(void)
{
	char *pwd;
	pwd = getcwd(NULL,0);

	printf("pwd %s\n",pwd);
}
