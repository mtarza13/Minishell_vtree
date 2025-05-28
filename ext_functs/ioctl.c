#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
	struct winsize w;
	
	ioctl(0 , TIOCGWINSZ,&w);
	printf("Row: %d cols %d\n",w.ws_row,w.ws_col);
}
