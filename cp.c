#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define BUFFER 64

void fileCopy (char *dir1, char *dir2) {
	int fd1 = open (dir1, O_RDONLY );
	int fd2 = open(dir2, O_WRONLY | O_CREAT, 0777 );
	char buffer[BUFFER + 1];
	int readQuantity;
	if (fd1 < 0 || fd2 < 0) {
		printf ("Error. Can't open the file\n");
		exit(1);
	}
	if ((readQuantity = read( fd1, &buffer, sizeof(buffer))) > 0) {
		if (write(fd2, &buffer, readQuantity) < 0) {
			printf ("Can't write\n");
			exit(1);
		}
	}
	close(fd1);
	close(fd2);
}

void directoryCopy(char *dir1, char *dir2) {
	DIR *path1, *pathr2;   
	struct dirent *into;
	struct stat statQ;
	path1 = opendir(dir1);   
	char nd1[ PATH_MAX + 1];
	char nd2[ PATH_MAX + 1];
	if (!path1) {
		printf ("Cannot open file '%s'\n", dir1);
		exit(1);
	}
	if (mkdir(dir2, 0777) < 0){
		printf("Cannot make directory '%s'\n", dir2);
		exit(1);
	}

	while ((into = readdir(path1)) != NULL) {
		if (into -> d_name[0] == '.')
		    continue;

		strncpy(nd1, dir1, PATH_MAX);
		strncpy(nd2, dir2, PATH_MAX);
		strncat(nd1, "/", PATH_MAX);
		strncat(nd2, "/", PATH_MAX);
		strncat(nd1, into -> d_name, PATH_MAX);
		strncat(nd2, into -> d_name, PATH_MAX);

		if (stat(nd1, &statQ) < 0)
		{
			fprintf (stderr, "ls: Cannot open file\n");
			exit(1);
		}

		if (statQ.st_mode & S_IFREG)
			fileCopy(nd1, nd2);

		if (statQ.st_mode & S_IFDIR)
			directoryCopy(nd1, nd2);
	}
	closedir(path1);
}

int main(int argc, char **argv) {
    
	struct stat statQ;
    
	if (argc < 3) {
		printf("Too few arguments\n");
		exit (1);
	}
    
	if (stat(argv[1] , &statQ) < 0) {
		printf(" Cannot open file\n");
		exit(1);
	}
    
	if (statQ.st_mode & S_IFREG)
		fileCopy (argv[1], argv[2]);
	else {
		if (opendir(argv[2])) {
			printf("This name already exists '%s'\n", argv[2]);
			exit(1);
		}
		
		directoryCopy (argv[1], argv[2]);
	}
	exit(0);
}

















