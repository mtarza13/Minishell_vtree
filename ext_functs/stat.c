#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void print_stat(struct stat sb, const char *type)
{
    printf("\n[%s] Info:\n", type);
    printf("Size: %ld bytes\n", sb.st_size);
    printf("Inode: %ld\n", sb.st_ino);
    printf("Permissions: %o\n", sb.st_mode & 0777);
    printf("Hard links: %ld\n", sb.st_nlink);
    printf("UID: %d | GID: %d\n", sb.st_uid, sb.st_gid);
    printf("Last modified: %ld\n", sb.st_mtime);
}

void handle_stat(const char *filename)
{
    struct stat sb;
    if (stat(filename, &sb) == -1)
        perror("stat");
    else
        print_stat(sb, "stat");
}

void handle_lstat(const char *filename)
{
    struct stat sb;
    if (lstat(filename, &sb) == -1)
        perror("lstat");
    else
        print_stat(sb, "lstat");
}

void handle_fstat(const char *filename)
{
    struct stat sb;
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open (for fstat)");
        return;
    }

    if (fstat(fd, &sb) == -1)
        perror("fstat");
    else
        print_stat(sb, "fstat");

    close(fd);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    handle_stat(argv[1]);
    handle_lstat(argv[1]);
    handle_fstat(argv[1]);

    return 0;
}

