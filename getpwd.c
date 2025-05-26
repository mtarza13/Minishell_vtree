#include <stdio.h>
#include <unistd.h>




int main(void)
{
	char s[1024];
	if(getcwd(s,sizeof(s)) != NULL)
	{
		printf("current working directory %s",s);
	}
	else
		printf("is the current");
	
}
