#include <stdio.h>
#include <unistd.h>


int main(void)
{
	char *pwd;
	pwd = getcwd(NULL,0);
	printf("%s",pwd);
	chdir("/usr/include");
	pwd = getcwd(NULL,0);
	printf("pwd after chdir %s\n",pwd);

}
