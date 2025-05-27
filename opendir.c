
//#include <dirent.h>
//#include <stdio.h>

//int main(int ac , char *av[])
//{
//	DIR *dir = opendir(".");
//
//	struct dirent *event;
//
//	while((event = readdir(dir)) != NULL)
//	{
//			printf("%d",event->d_type);
//	}
//	closedir(dir);
//}
#include <dirent.h>
#include <stdio.h>

int main(void)
{
	DIR *dir = opendir(".");
	if (!dir) {
		perror("opendir");
		return (1);
	}

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL)
	{
		printf("Name: %s\tType: ", entry->d_name);
		if(entry->d_type == DT_REG)
			printf("File\n");
		else if(entry->d_type == DT_DIR)
			printf("Directory");
		else if(entry->d_type == DT_LNK)
			printf("Symbolink\n");
		else
				printf("Other (%d)\n", entry->d_type);
		}

	closedir(dir);

}

