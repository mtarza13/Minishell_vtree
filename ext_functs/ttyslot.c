#include <unistd.h>
#include <stdio.h>


int main()
{
	int i ; 
	i = ttyslot();
	printf("ttyslot is %d",i);
}
